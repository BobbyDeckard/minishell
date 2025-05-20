/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:33:22 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 15:54:32 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	*init_pids(int count)
{
	int	*pids;
	int	i;

	pids = (int *) malloc(count * sizeof(int));
	if (!pids)
	{
		perror("malloc");
		return (NULL);
	}
	i = -1;
	while (++i < count)
		pids[i] = -1;
	return (pids);
}

void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i)
{
	int	j;

	if (cmd1->type == NODE_CMD)
		cmd1->cmd.fd_out = fd[i % 2][1];
	else
	{
		j = -1;
		while (cmd1->children[++j])
			cmd1->children[j]->cmd.fd_out = fd[i % 2][1];
	}
	if (cmd2->type == NODE_CMD)
		cmd2->cmd.fd_in = fd[i % 2][0];
	else
	{
		j = -1;
		while (cmd2->children[++j])
			cmd2->children[j]->cmd.fd_in = fd[i % 2][0];
	}
}

void	exec_pipe_child(t_ast *node)
{
	if (node->type == NODE_CMD)
		exec_pipe_cmd(node);
	else if (node->type == NODE_AND_IF)
		exec_pipe_and(node);
	else if (node->type == NODE_OR_IF)
		exec_pipe_or(node);
}
