/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:05:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/04/27 19:08:08 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	dup_fds(t_ast node)
{

	if (node.cmd.fd_in != STDIN_FILENO && node.cmd.fd_in >= 0)
	{
		if (dup2(node.cmd.fd_in, STDIN_FILENO) == -1)
			dup2_error();
		close(node.cmd.fd_in);
	}
	if (node.cmd.fd_out != STDOUT_FILENO && node.cmd.fd_out >= 0)
	{
		if (dup2(node.cmd.fd_out, STDOUT_FILENO) == -1)
			dup2_error();
		close(node.cmd.fd_out);
	}
}
