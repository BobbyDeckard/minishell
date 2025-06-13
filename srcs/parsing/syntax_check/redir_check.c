/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/06/13 16:30:38 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
** Version ULTRA-STRICTE de redir_syntax_is_valid
** qui refuse absolument les redirections sans fichier
*/

int	is_redirection_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == REDIR_APPEND || token->type == HEREDOC);
}

int	is_valid_filename(t_token *token)
{
	if (!token)
		return (0);
	if (token->type != WORD)
		return (0);
	if (!token->content || token->content[0] == '\0')
		return (0);
	return (1);
}

int	redir_syntax_is_valid(t_token **token_list)
{
	t_token	*current;

	if (!token_list || !*token_list)
		return (1);
	current = *token_list;
	while (current)
	{
		if (is_redirection_token(current))
		{
			if (!is_valid_filename(current->next))
				return (0);
		}
		current = current->next;
	}
	return (1);
}