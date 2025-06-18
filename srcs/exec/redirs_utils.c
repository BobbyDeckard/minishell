/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:44:32 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/18 19:25:44 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	close_redirs(t_cmd cmd)
{
	if (cmd.fd_in != STDIN_FILENO && cmd.fd_in >= 0)
		close(cmd.fd_in);
	if (cmd.fd_out != STDOUT_FILENO && cmd.fd_out >= 0)
		close(cmd.fd_out);
}

int	check_redirs(t_ast *node, t_cmd cmd)
{
	(void) node;
	if (cmd.fd_in < 0 || cmd.fd_out < 0)
	{
		close_redirs(cmd);
		unlink_heredoc(node);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

