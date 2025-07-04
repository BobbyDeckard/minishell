/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 21:22:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/11 16:26:39 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	exec_pipe_cmd(t_ast *node)
{
	setup_child_signals();
	if (make_redirs(node) == FAILURE)
		clean_exit(node->root, FAILURE);
	dup_fds(*node);
	exec_cmd(node, node->cmd);
	clean_exit(node->root, FAILURE);
}

void	exec_pipe_and(t_ast *node)
{
	int	status;
	int	pid;
	int	i;

	i = -1;
	while (node->children[++i])
	{
		if (node->children[i]->type == NODE_CMD)
		{
			pid = make_fork();
			if (pid == 0)
				exec_pipe_cmd(node->children[i]);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			if (status != SUCCESS)
				break ;
		}
		else
			exec_pipe_child(node->children[i]);
	}
}

void	exec_pipe_or(t_ast *node)
{
	int	status;
	int	pid;
	int	i;

	i = -1;
	while (node->children[++i])
	{
		if (node->children[i]->type == NODE_CMD)
		{
			pid = make_fork();
			if (pid == 0)
				exec_pipe_cmd(node->children[i]);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			if (status == SUCCESS)
				break ;
		}
		else
			exec_pipe_child(node->children[i]);
	}
}

int	run_pipe(t_ast **child, int *pids, int count)
{
	int	fd[2][2];
	int	i;

	i = -1;
	while (++i < count)
	{
		if (i + 1 < count) 
		{
			if (make_pipe(fd[i % 2]))
				link_pipe(child[i], child[i + 1], fd, i);
			else
				return (pipe_error(pids, fd, i, count));
		}
		if (child[i]->type == NODE_CMD && is_builtin(child[i]->cmd))
			exec_builtin(child[i]);
		else
			pids[i] = make_fork();
		if (pids[i] == 0)
			exec_pipe_child(child[i]);
		close_pipes(fd, i, count);
	}
	return (waitpids(pids, count));
}

int	exec_pipe(t_ast **children)
{
	int		*pids;
	int		status;
	int		count;

	count = count_nodes(children);
	pids = init_pids(count);
	if (!pids)
		return (FAILURE);
	status = run_pipe(children, pids, count);
	free(pids);
	return (status);
}