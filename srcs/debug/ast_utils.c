/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:25:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 15:28:28 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// recursively sets the root pointer of each node to the tree's root node
void	set_root_node(t_ast *ast, t_ast *root)
{
	int	i;

	ast->root = root;
	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			set_root_node(ast->children[i], root);
	}
}
