/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:31:04 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/17 21:04:32 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	echo(t_ast	*node)
{
	int flag;
	int	i;

	if (make_redirs(node) == EXIT_FAILURE)
		return (set_exit_status(node, EXIT_FAILURE));
	flag = 0;
	if (!ft_strncmp(node->cmd.args[1], "-n", node->cmd.fd_out))
		flag++;
	i = flag;
	while (node->cmd.args[++i])
	{
		ft_putstr_fd(node->cmd.args[i], node->cmd.fd_out);
		if (node->cmd.args[i + 1])
			ft_putchar_fd(' ', node->cmd.fd_out);
	}
	if (!flag)
		ft_putchar_fd('\n', node->cmd.fd_out);
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, EXIT_SUCCESS));
}
