/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/13 16:24:47 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
** Version ULTRA-STRICTE de quote_syntax_is_valid
** qui refuse absolument les guillemets non fermés
*/
int	count_quote_tokens(t_token **token_list, t_token_type quote_type)
{
	t_token	*current;
	int		count;

	if (!token_list || !*token_list)
		return (0);
	count = 0;
	current = *token_list;
	while (current)
	{
		if (current->type == quote_type)
			count++;
		current = current->next;
	}
	return (count);
}

int	quote_syntax_is_valid(t_token **token_list)
{
	int	single_quote_count;
	int	double_quote_count;

	if (!token_list || !*token_list)
		return (1);
	single_quote_count = count_quote_tokens(token_list, SINGLE_QUOTE);
	double_quote_count = count_quote_tokens(token_list, DOUBLE_QUOTE);
	if (single_quote_count % 2 != 0)
		return (0);
	if (double_quote_count % 2 != 0)
		return (0);
	return (1);
}