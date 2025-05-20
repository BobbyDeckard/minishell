/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:34:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 20:13:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*cd_error(t_ast	*node)
{
	t_cmd	cmd;
	char	*msg;
	int		len;

	cmd = node->cmd;
	len = ft_strlen(cmd.args[1]);
	msg = (char *) malloc((len + 5) * sizeof(char));
	if (!msg)
		clean_exit(node->root, FAILURE);
	ft_strlcpy(msg, "cd: ", len + 5);
	ft_strlcat(msg, cmd.args[1], len + 5);
	return (msg);
}

int	cd(t_ast *node)
{
	char	*error;

	// redirs with cd ? is that even possible ?
	if (make_redirs(node) == FAILURE)
		return (FAILURE);
	if (chdir(node->cmd.args[1]) < 0)
	{
		error = cd_error(node);
		perror(error);
		free(error);
		close_redirs(node->cmd);
		unlink_heredoc(node);
		return (FAILURE);
	}
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (SUCCESS);
}
