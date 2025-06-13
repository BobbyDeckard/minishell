/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:54:09 by pitran            #+#    #+#             */
/*   Updated: 2025/06/13 16:35:55 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
** Version ULTRA-AGRESSIVE qui REFUSE absolument les 4 cas problématiques
*/
int	check_pipe_position(t_token **token_list)
{
	t_token	*first;
	t_token	*last;

	if (!token_list || !*token_list)
		return (1);
	first = *token_list;
	if (first->type == PIPE)
		return (0);
	last = first;
	while (last->next)
		last = last->next;
	if (last->type == PIPE)
		return (0);
	return (1);
}

int	check_redir_target(t_token **token_list)
{
	t_token	*current;

	if (!token_list || !*token_list)
		return (1);
	current = *token_list;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_IN
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			if (!current->next || current->next->type != WORD)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_quote_pairs(t_token **token_list)
{
	t_token	*current;
	int		single_count;
	int		double_count;

	if (!token_list || !*token_list)
		return (1);
	single_count = 0;
	double_count = 0;
	current = *token_list;
	while (current)
	{
		if (current->type == SINGLE_QUOTE)
			single_count++;
		if (current->type == DOUBLE_QUOTE)
			double_count++;
		current = current->next;
	}
	if (single_count % 2 != 0 || double_count % 2 != 0)
		return (0);
	return (1);
}

int	syntax_is_valid(t_token **token_list)
{
	if (!token_list || !*token_list)
		return (1);
	if (!check_pipe_position(token_list))
	{
		printf("Error: Invalid pipe position\n");
		return (0);
	}
	if (!check_redir_target(token_list))
	{
		printf("Error: Invalid redirection syntax\n");
		return (0);
	}
	if (!check_quote_pairs(token_list))
	{
		printf("Error: Unmatched quotes\n");
		return (0);
	}
	if (!paren_syntax_is_valid(token_list))
	{
		printf("Error: Unmatched parentheses\n");
		return (0);
	}
	return (1);
}