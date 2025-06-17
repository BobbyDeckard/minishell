/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:37:06 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/17 21:04:11 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
void	exec_pipe_builtin(t_ast *node, int *pids, int i)
{
	char	*name;

	if (!node->cmd.args)
		pids[i] = FAILURE;
	name = node->cmd.args[0];
	if (!ft_strncmp(name, "echo", 5))
		pids[i] = echo(node);
	else if (!ft_strncmp(name, "cd", 3))
		pids[i] = cd(node);
	else if (!ft_strncmp(name, "pwd", 4))
		pids[i] = pwd(node);
	else if (!ft_strncmp(name, "export", 7))
		pids[i] = export_bltn(node);
	else if (!ft_strncmp(name, "unset", 6))
		pids[i] = unset(node);
	else if (!ft_strncmp(name, "env", 4))
		pids[i] = env(node);
	else if (!ft_strncmp(name, "exit", 5))
	{
		pids[i] = SUCCESS;
		exit(42);
	}
	else
		pids[i] = FAILURE;
}
*/

int	exec_builtin(t_ast *node)
{
	char	*name;

	if (!node->cmd.args)
		return (EXIT_FAILURE);
	name = node->cmd.args[0];
	if (!ft_strncmp(name, "echo", 5))
		return (echo(node));
	else if (!ft_strncmp(name, "cd", 3))
		return (cd(node));
	else if (!ft_strncmp(name, "pwd", 4))
		return (pwd(node));
	else if (!ft_strncmp(name, "export", 7))
		return (export_bltn(node));
	else if (!ft_strncmp(name, "unset", 6))
		return (unset(node));
	else if (!ft_strncmp(name, "env", 4))
		return (env(node));
	else if (!ft_strncmp(name, "exit", 5))
		return (exit_bltn(node), EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	is_builtin(t_cmd cmd)
{
	char	*name;

	if (!cmd.args)
		return (0);
	name = cmd.args[0];
	if (!ft_strncmp(name, "echo", 5))
		return (1);
	else if (!ft_strncmp(name, "cd", 3))
		return (1);
	else if (!ft_strncmp(name, "pwd", 4))
		return (1);
	else if (!ft_strncmp(name, "export", 7))
		return (1);
	else if (!ft_strncmp(name, "unset", 6))
		return (1);
	else if (!ft_strncmp(name, "env", 4))
		return (1);
	else if (!ft_strncmp(name, "exit", 5))
		return (1);
	return (0);
}
