/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:30:21 by pitran            #+#    #+#             */
/*   Updated: 2025/06/10 17:37:19 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"
#include "../../../incl/exec.h"

/* Je retire la node root et renvoie la node tree à la place */
t_ast	*create_ast(t_token **token_list)
{
	t_ast	*root;
	t_ast	*tree;
	int		token_count;

	if (!token_list || !*token_list)
		return (NULL);

	// jsp si c'est nécessaire de garder la node root
	root = create_ast_node(NODE_CMD, NULL);
	if (!root)
		return (NULL);
	root->root = root;
//	root->envp = g_shell.envp;

	token_count = count_tokens(token_list);
	tree = parse_command_line(token_list, 0, token_count - 1, root);
	if (!tree)
	{
		free_ast(root);
		return (NULL);
	}
//	tree->envp = g_shell.envp;
	free(root);
	set_root_node(tree, tree);		// fonction qui initialise le pointeur root
									// de toutes les nodes de l'arbre à la node
									// passée en 2e argument
	return (tree);
}

/*
t_ast	*create_ast(t_token **token_list)
{
	t_ast	*root;
	t_ast	*tree;
	int		token_count;

	if (!token_list || !*token_list)
		return (NULL);
	root = create_ast_node(NODE_CMD, NULL);
	if (!root)
		return (NULL);
	root->root = root;
	root->envp = g_shell.envp;
	token_count = count_tokens(token_list);
	tree = parse_command_line(token_list, 0, token_count - 1, root);
	if (!tree)
	{
		free_ast(root);
		return (NULL);
	}
	if (tree->type == NODE_CMD)
	{
		tree->root = tree;
		tree->envp = g_shell.envp;
		free(root);
		return (tree);
	}
	root->children = (t_ast **)malloc(sizeof(t_ast *) * 2);
	if (!root->children)
	{
		free_ast(tree);
		free_ast(root);
		return (NULL);
	}
	root->children[0] = tree;
	root->children[1] = NULL;
	return (root);
}
*/

t_token	*get_token_at_index(t_token **token_list, int index)
{
	t_token	*current;
	int		i;

	if (!token_list || !*token_list || index < 0)
		return (NULL);
	current = *token_list;
	i = 0;
	while (current && i < index)
	{
		current = current->next;
		i++;
	}
	return (current);
}

int	matching_parentheses(t_token **tokens, int start, int end)
{
	int		level;
	int		i;
	t_token	*current;

	current = get_token_at_index(tokens, start);
	if (!current || current->type != PAREN_OPEN)
		return (0);
	current = get_token_at_index(tokens, end);
	if (!current || current->type != PAREN_CLOSE)
		return (0);
	level = 0;
	i = start;
	while (i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (current->type == PAREN_OPEN)
			level++;
		else if (current->type == PAREN_CLOSE)
		{
			level--;
			if (level == 0 && i != end)
				return (0);
		}
		i++;
	}
	return (level == 0);
}

int	find_matching_parenthesis(t_token **tokens, int open_pos, int end)
{
	int		level;
	int		i;
	t_token	*current;

	current = get_token_at_index(tokens, open_pos);
	if (!current || current->type != PAREN_OPEN)
		return (-1);
	level = 1;
	i = open_pos + 1;
	while (i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (current->type == PAREN_OPEN)
			level++;
		else if (current->type == PAREN_CLOSE)
		{
			level--;
			if (level == 0)
				return (i);
		}
		i++;
	}
	return (-1);
}
