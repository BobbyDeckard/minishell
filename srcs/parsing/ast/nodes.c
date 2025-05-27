/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:34:09 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:40:43 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_ast	*create_ast_node(t_node_type type, t_ast *root)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = NULL;
	node->children = NULL;
	node->root = root;
	node->envp = NULL;
	node->paths = NULL;
	node->cmd.args = NULL;
	node->cmd.path = NULL;
	node->cmd.fd_in = STDIN_FILENO;
	node->cmd.fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_command_node(char **args, t_ast *root)
{
	t_ast	*node;

	node = create_ast_node(NODE_CMD, root);
	if (!node)
		return (NULL);
	node->cmd.args = args;
	return (node);
}

t_ast	*create_operator_node(t_node_type type, t_ast *left, t_ast *right,
	t_ast *root)
{
	t_ast	*node;

	node = create_ast_node(type, root);
	if (!node)
		return (NULL);
	node->children = (t_ast **)malloc(sizeof(t_ast *) * 3);
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	node->children[0] = left;
	node->children[1] = right;
	node->children[2] = NULL;
	return (node);
}

t_ast	*create_subshell_node(t_ast *child, t_ast *root)
{
	t_ast	*node;

	node = create_ast_node(NODE_SUBSHELL, root);
	if (!node)
		return (NULL);
	node->children = (t_ast **)malloc(sizeof(t_ast *) * 2);
	if (!node->children)
	{
		free(node);
		return (NULL);
	}
	node->children[0] = child;
	node->children[1] = NULL;
	return (node);
}

t_ast	*create_redirection_node(t_node_type type, char *file, t_ast *root)
{
	t_ast	*node;

	node = create_ast_node(type, root);
	if (!node)
		return (NULL);
	node->file = file;
	return (node);
}
