/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_type_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:51:19 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:40:57 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int quote_syntax_is_valid(t_token **token_list)
{
	int in_single_quote;
	int in_double_quote;
	t_token *current;

	in_single_quote = 0;
	in_double_quote = 0;
	current = *token_list;
	while (current)
	{
		if (current->type == SINGLE_QUOTE && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (current->type == DOUBLE_QUOTE && !in_single_quote)
			in_double_quote = !in_double_quote;
		current = current->next;
	}
	return (!in_single_quote && !in_double_quote);
}

int	redir_syntax_is_valid(t_token **token_list)
{
	t_token *current;

	current = *token_list;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT || current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			if (!current->next || current->next->type != WORD)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	operator_syntax_is_valid(t_token **token_list)
{
	t_token	*current;

	current = *token_list;
	while (current)
	{
		if (current->type == PIPE || current->type == AND || current->type == OR)
		{
			if (!current->prev || !current->next)
				return (0);			
			if (current->prev->type != WORD && current->prev->type != PAREN_CLOSE)
				return (0);
			if (current->next->type != WORD && current->next->type != PAREN_OPEN)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
