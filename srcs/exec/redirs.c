/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:54:50 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 20:12:47 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	make_redir_in(t_ast *node, t_cmd *cmd)
{
	if (access(node->file, F_OK) != 0 || access(node->file, R_OK) != 0)
		cmd->fd_in = -1;
	else
		cmd->fd_in = open(node->file, O_RDONLY);
	if (cmd->fd_in < 0)
		perror(node->file);
}

void	make_redir_out(t_ast *node, t_cmd *cmd)
{
	if (access(node->file, F_OK) == 0 && access(node->file, W_OK) != 0)
		cmd->fd_out = -1;
	else
		cmd->fd_out = open(node->file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (cmd->fd_out < 0)
		perror(node->file);
}

void	make_redir_append(t_ast *node, t_cmd *cmd)
{
	if (access(node->file, F_OK) == 0 && access(node->file, W_OK) != 0)
		cmd->fd_out = -1;
	else
		cmd->fd_out = open(node->file, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (cmd->fd_out < 0)
		perror(node->file);
}

int	make_redirs(t_ast *node)
{
	int	i;

	i = -1;
	if (node->children)
	{
		while (node->children[++i])
		{
			if (node->children[i]->type == NODE_REDIR_IN)
				make_redir_in(node->children[i], &node->cmd);
			else if (node->children[i]->type == NODE_REDIR_OUT)
				make_redir_out(node->children[i], &node->cmd);
			else if (node->children[i]->type == NODE_REDIR_APPEND)
				make_redir_append(node->children[i], &node->cmd);
			else if (node->children[i]->type == NODE_HEREDOC)
				make_heredoc(node->children[i], &node->cmd);
		}
	}
	return (check_redirs(node, node->cmd));
}
