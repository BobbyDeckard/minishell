/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:00:23 by pitran            #+#    #+#             */
/*   Updated: 2025/06/11 18:56:26 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

//Token type assignment
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

//Token list management 
t_token	**tokenize_input(char *command)
{
	t_token		**token_list;
	t_token		**result;
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
		{
			free(token_list);
			return (NULL);
		}
		token_list = result;
	}
	mark_tokens_for_expansion(token_list);  
	return (token_list);
}

//Token malloc + link
t_token	**extract_token(char **command, t_token_type type, t_token **token_list)
{
	t_token	*new_token;

	if (!*(command) || !command || type == DEFAULT)
		return (NULL);
	new_token = create_token();
	if (!new_token)
		return (token_list);//Message d'erreur
	new_token = handle_token_type(command, type, new_token);
	if (new_token)
		link_token(new_token, token_list);//If/ else avec free 
	return (token_list);
}

//Mother function to call type handlers
t_token	*handle_token_type(char **command, t_token_type type, t_token *new_token)
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

int	token_should_expand(t_token *token)
{
	char	*content;
	int		has_variables;
	int		in_single_quotes;
	int		len;
	int		i;

	if (!token || !token->content || token->type != WORD)
		return (0);
	content = token->content;
	len = ft_strlen(content);
	in_single_quotes = (len >= 2 && content[0] == '\'' 
		&& content[len - 1] == '\'');
	if (in_single_quotes)
		return (0);
	has_variables = 0;
	i = 0;
	while (content[i])
	{
		if (content[i] == '$')
		{
			has_variables = 1;
			break;
		}
		i++;
	}
	return (has_variables);
}

void	mark_tokens_for_expansion(t_token **token_list)
{
	t_token	*current;

	if (!token_list || !*token_list)
		return;
	current = *token_list;
	while (current)
	{
		if (current->type == WORD)
			current->needs_expansion = token_should_expand(current);
		else
			current->needs_expansion = 0;
		current = current->next;
	}
}
