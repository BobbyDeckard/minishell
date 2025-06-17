/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:22:05 by pitran            #+#    #+#             */
/*   Updated: 2025/06/17 15:33:04 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"
t_ast	*parse_input(char *command, t_shell_data *shell)
{
	t_token	**token_list;
	t_ast	*ast_root;
	int		syntax_valid;

	(void)shell;
	if (!command)
		return (NULL);
	token_list = tokenize_input(command);
	if (!token_list)
		return (NULL);
	syntax_valid = syntax_is_valid(token_list);
	if (!syntax_valid)
	{
		free_token_list(token_list);
		free(token_list);  // Libérer le pointeur token_list
		return (printf("Syntax error in command\n"), NULL);
	}
	ast_root = create_ast(token_list);
	if (!ast_root)
	{
		free_token_list(token_list);
		free(token_list); 
		return (printf("Failed to create AST\n"), NULL);
	}
	free_token_list(token_list);
	free(token_list);
	return (ast_root);
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
