/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:51:08 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/12 15:42:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	env(t_ast *node)
{
	int	i;

	if (make_redirs(node) == FAILURE)
		return (set_exit_status(node, FAILURE));
	i = -1;
	while (node->root->envp[++i])
	{
		ft_putstr_fd(node->root->envp[i], node->cmd.fd_out);
		ft_putchar_fd('\n', node->cmd.fd_out);
	}
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, SUCCESS));
}
