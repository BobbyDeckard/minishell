/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:54:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/13 17:21:49 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	redir_error(t_ast *node);

int	exec_solo_redir_in(t_ast *node)
{
	if (access(node->file, F_OK) != 0 || access(node->file, R_OK) != 0)
	{
		redir_error(node);
		return (set_exit_status(node, FAILURE));
	}
	return (set_exit_status(node, SUCCESS));
}

int	exec_solo_redir_out(t_ast *node)
{
	int	fd;

	if (access(node->file, F_OK) == 0 && access(node->file, W_OK) != 0)
	{
		redir_error(node);
		return (set_exit_status(node, FAILURE));
	}
	fd = open(node->file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
	{
		redir_error(node);
		return (set_exit_status(node, FAILURE));
	}
	close(fd);
	return (set_exit_status(node, SUCCESS));
}

int	exec_solo_redir_append(t_ast *node)
{
	int	fd;

	if (access(node->file, F_OK) == 0 && access(node->file, W_OK) != 0)
	{
		redir_error(node);
		return (set_exit_status(node, FAILURE));
	}
	fd = open(node->file, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
	{
		redir_error(node);
		return (set_exit_status(node, FAILURE));
	}
	close(fd);
	return (set_exit_status(node, SUCCESS));
}

int	exec_solo_heredoc(t_ast *node)
{
	printf("ntm\n");
	return (set_exit_status(node, SUCCESS));
}
