/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:51:08 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/12 15:07:16 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	create_envp(t_ast *node)
{
	char	*cwd;
	int		len;

	free(node->root->envp);
	node->root->envp = (char **) malloc(4 * sizeof(char *));
	if (!node->root->envp)
		malloc_error(node);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	node->root->envp[0] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[0])
		malloc_error(node);
	ft_strlcat(node->root->envp[0], "PWD=", len);
	ft_strlcat(node->root->envp[0], cwd, len);
	free(cwd);
	node->root->envp[1] = NULL;	// need to address the two other variables too
	return (SUCCESS);
}

// env -i bash still prints PWD, _ and SHLVL...
int	env(t_ast *node)
{
	int	i;

	if (make_redirs(node) == FAILURE)
		return (set_exit_status(node, FAILURE));
	if (!*node->root->envp)
		return (create_envp(node));
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
