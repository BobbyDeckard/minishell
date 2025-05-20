/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:16:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 20:14:15 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	pwd(t_ast *node)
{
	char	*cwd;

	if (make_redirs(node) == FAILURE)
		return (FAILURE);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		close_redirs(node->cmd);
		unlink_heredoc(node);
		return (FAILURE);
	}
	ft_putstr_fd(cwd, node->cmd.fd_out);
	ft_putchar_fd('\n', node->cmd.fd_out);
	free(cwd);
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (SUCCESS);
}
