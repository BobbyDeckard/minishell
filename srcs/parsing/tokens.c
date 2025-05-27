/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:59:05 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:41:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

//Token allocation and initialisation
t_token	*create_token(void)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = 0;
	new_token->content = NULL;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

//Link node 
void	link_token(t_token *token, t_token **token_list)
{
	t_token	*current;

	if (!token || !token_list)
		return ;
	if (!*token_list)
	{
		*token_list = token;
		return ;
	}
	current = *token_list;
	while (current->next)
		current = current->next;
	current->next = token;
	token->prev = current;
}

void	free_token(t_token *token)
{
	token->type = DEFAULT;
	free(token->content);
	token->content = NULL;
	token->next = NULL;
	token->prev = NULL;
	free(token);
	token = NULL;
}

void	free_token_list(t_token **token_list)
{
	t_token	*current;

	current = *token_list;
	while (current)
	{
		free_token(current);
		current = current->next;
	}
}
