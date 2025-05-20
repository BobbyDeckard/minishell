/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 00:30:32 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 20:09:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	exec_cmd(t_ast *node, t_cmd cmd)
{
	get_cmd_path(&cmd, node->root->paths);
	if (!cmd.path)
	{
		ft_putstr_fd(cmd.args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(1);
	}
	if (execve(cmd.path, cmd.args, node->root->envp) == -1)
		perror("execve");
}

int	run_cmd(t_ast *node)
{
	int		status;
	int		pid;

	status = -1;
	if (is_builtin(node->cmd))
		return (exec_builtin(node));
	if (make_redirs(node) == FAILURE)
		return (FAILURE);
	pid = fork();
	if (pid < 0)
		return (fork_error());
	if (pid == 0)
	{
		dup_fds(*node);
		exec_cmd(node, node->cmd);
		unlink_heredoc(node);
		clean_exit(node->root, FAILURE);
	}
	close_redirs(node->cmd);
	waitpid(pid, &status, 0);
	unlink_heredoc(node);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

int	exec_or_if(t_ast **child)
{
	int	i;

	i = -1;
	while (child[++i])
	{
		if (exec_ast(child[i]) == SUCCESS)
			return (SUCCESS);
	}
	return (FAILURE);
}

int	exec_and_if(t_ast **child)
{
	int	i;

	i = -1;
	while (child[++i])
	{
		if (exec_ast(child[i]) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	exec_ast(t_ast *node)
{
	if (node->type == NODE_CMD)
		return (run_cmd(node));
	else if (node->type == NODE_OR_IF && node->children)
		return (exec_or_if(node->children));
	else if (node->type == NODE_AND_IF && node->children)
		return (exec_and_if(node->children));
	else if (node->type == NODE_PIPE && node->children)
		return (exec_pipe(node->children));
	else if (node->type == NODE_SUBSHELL)
		return (exec_subshell(node));
	return (FAILURE);
}
