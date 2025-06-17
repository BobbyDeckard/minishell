/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:51:08 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/17 21:04:52 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	env(t_ast *node)
{
	int	i;

	if (make_redirs(node) == EXIT_FAILURE)
		return (set_exit_status(node, EXIT_FAILURE));
	i = -1;
	while (node->root->envp[++i])
	{
		if (!ft_strncmp(node->root->envp[i], "_=", 2))
		{
			free(node->root->envp[i]);
			node->root->envp[i] = (char *) malloc(15 * sizeof(char));
			if (!node->root->envp[i])
				malloc_error(node);
			ft_strlcat(node->root->envp[i], "_=/usr/bin/env", 15);
		}
		// ajouter condition si variable a été assignée ou non, surtout pour OLDPWD
		ft_putstr_fd(node->root->envp[i], node->cmd.fd_out);
		ft_putchar_fd('\n', node->cmd.fd_out);
	}
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, EXIT_SUCCESS));
}
