/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:22:05 by pitran            #+#    #+#             */
/*   Updated: 2025/05/20 14:20:10 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_input(char *command)
{
	t_token	**token_list;
	t_ast	*ast_root;
	int		syntax_valid;

	if (!command)
		return (1);
	token_list = tokenize_input(command);
	if (!token_list)
		return (1);
	syntax_valid = syntax_is_valid(token_list);
	if (!syntax_valid)
	{
		free_token_list(token_list);
		return (printf("Syntax error in command\n"), 1);
	}
	print_token_list(token_list);
	ast_root = create_ast(token_list);
	if (!ast_root)
	{
		free_token_list(token_list);
		return (printf("Failed to create AST\n"), 1);
	}
	printf("\n--- AST Structure ---\n");
	print_ast(ast_root, 0);
	free_ast(ast_root);
	free_token_list(token_list);
	return (0);
}

void	print_token_list(t_token **token_list)
{
	t_token	*current;
	int		token_count;

	token_count = 0;
	if (!token_list || !*token_list)
	{
		printf("No tokens to display\n");
		return ;
	}
	printf("--- Token List ---\n");
	current = *token_list;
	while (current)
	{
		printf("Token %d --> Type: %d, Content: '%s'\n",
			token_count++, current->type, current->content);
		current = current->next;
	}
}