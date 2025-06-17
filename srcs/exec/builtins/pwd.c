/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:16:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/17 21:06:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	pwd(t_ast *node)
{
	char	*cwd;

	if (make_redirs(node) == EXIT_FAILURE)
		return (set_exit_status(node, EXIT_FAILURE));
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		close_redirs(node->cmd);
		unlink_heredoc(node);
		return (set_exit_status(node, EXIT_FAILURE));
	}
	ft_putstr_fd(cwd, node->cmd.fd_out);
	ft_putchar_fd('\n', node->cmd.fd_out);
	free(cwd);
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, EXIT_SUCCESS));
}
