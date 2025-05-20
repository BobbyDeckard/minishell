/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:56:56 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 15:53:53 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	make_fork(void)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		fork_error();
	return (pid);
}

int	make_pipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

int	waitpids(int *pids, int cmd_count)
{
	int	status;
	int	i;

	status = -1;
	i = -1;
	while (++i < cmd_count)
	{
		if (pids[i] < 0)
			status = FAILURE;
		else
			waitpid(pids[i], &status, 0);
	}
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

int	count_nodes(t_ast **children)
{
	int	i;

	i = 0;
	while (children[i])
		i++;
	return (i);
}

void	close_pipes(int fd[2][2], int i, int count)
{
	if (i + 1 < count)
		close(fd[i % 2][1]);
	if (i > 0)
		close(fd[(i + 1) % 2][0]);
}
