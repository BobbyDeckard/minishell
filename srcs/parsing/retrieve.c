/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:00:23 by pitran            #+#    #+#             */
/*   Updated: 2025/06/12 16:20:09 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

t_token_type	get_token_type(char c)
{
	if (is_command_char(c))
		return (WORD);
	else if (is_quote(c))
		return (QUOTE);
	else if (is_redirection(c))
		return (REDIR);
	else if (is_operator(c))
		return (OPERATOR);
	else if (is_parenthesis(c))
		return (PARENTHESIS);
	else if (is_special_caracter(c))
		return (SPECIAL_CARACTER);
	return (DEFAULT);
}

t_token	**tokenize_input(char *command)
{
	t_token			**token_list;
	t_token			**result;
	t_token_type	type;

	token_list = malloc(sizeof(t_token *));
	if (!token_list)
		return (NULL);
	*token_list = NULL;
	while (*command)
	{
		type = get_token_type(*command);
		if (type == DEFAULT)
		{
			command++;
			continue ;
		}
		result = extract_token(&command, type, token_list);
		if (!result)
			return (free(token_list), NULL);
		token_list = result;
	}
	mark_tokens_for_expansion(token_list);
	return (token_list);
}

t_token	**extract_token(char **command, t_token_type type, t_token **token_list)
{
	t_token	*new_token;

	if (!*(command) || !command || type == DEFAULT)
		return (NULL);
	new_token = create_token();
	if (!new_token)
		return (token_list);
	new_token = handle_token_type(command, type, new_token);
	if (new_token)
		link_token(new_token, token_list);
	return (token_list);
}

t_token	*handle_token_type(char **command, t_token_type type,
							t_token *new_token)
{
	if (type == WORD)
		new_token = tokenize_word(command, new_token);
	else if (type == QUOTE)
		new_token = tokenize_quote(command, new_token);
	else if (type == REDIR)
		new_token = tokenize_redir(command, new_token);
	else if (type == OPERATOR)
		new_token = tokenize_operator(command, new_token);
	else if (type == PARENTHESIS)
		new_token = tokenize_parenthesis(command, new_token);
	else if (type == SPECIAL_CARACTER)
		new_token = tokenize_special_caracter(command, new_token);
	return (new_token);
}
