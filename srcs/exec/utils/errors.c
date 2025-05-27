/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:51:47 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/27 14:51:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

// Returns FAILURE to keep the rest of the execution running.
int	fork_error(void)
{
	perror("fork");
	return (FAILURE);
}

// Here we can use exit() because dup2() is only called in child processes.
void	dup2_error(void)
{
	perror("dup2");
	exit(FAILURE);
}

int	pipe_error(int *pids, int fd[2][2], int i, int count)
{
	close_pipes(fd, i, count);
	waitpids(pids, count);
	return (FAILURE);
}

// appelée en cas d'erreur de malloc, met fin à la séquence en cours
void	malloc_error(t_ast *node)
{
	perror("malloc");
	if (node)
		clean_exit(node->root, FAILURE);
}
