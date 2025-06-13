/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/13 16:24:43 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
** Version ULTRA-STRICTE de operator_syntax_is_valid
** qui refuse absolument les pipes en début/fin
*/

int	is_operator_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == PIPE || token->type == AND || token->type == OR);
}

int	is_valid_operand(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == WORD || token->type == PAREN_CLOSE
		|| token->type == PAREN_OPEN);
}

int	operator_syntax_is_valid(t_token **token_list)
{
	t_token	*current;
	t_token	*first;
	t_token	*last;

	if (!token_list || !*token_list)
		return (1);
	first = *token_list;
	current = first;
	while (current && current->next)
		current = current->next;
	last = current;
	if (is_operator_token(first))
		return (0);
	if (is_operator_token(last))
		return (0);
	current = *token_list;
	while (current)
	{
		if (is_operator_token(current))
		{
			if (!is_valid_operand(current->prev))
				return (0);
			if (!is_valid_operand(current->next))
				return (0);
		}
		current = current->next;
	}
	return (1);
}