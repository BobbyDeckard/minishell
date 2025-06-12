/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_detection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:00:00 by pitran            #+#    #+#             */
/*   Updated: 2025/06/12 16:24:01 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/*
** ========================================================================
** DÉTECTION DES VARIABLES DANS UN WORD
** ========================================================================
** Vérifie si un token WORD contient des variables à expander
*/
int	word_contains_variables(char *content)
{
	int	i;
	int	in_single_quotes;

	if (!content)
		return (0);
	in_single_quotes = 0;
	i = 0;
	while (content[i])
	{
		if (content[i] == '\'' && !in_single_quotes)
			in_single_quotes = 1;
		else if (content[i] == '\'' && in_single_quotes)
			in_single_quotes = 0;
		else if (content[i] == '$' && !in_single_quotes)
		{
			if (content[i + 1] == '?' || ft_isalpha(content[i + 1])
				|| content[i + 1] == '_')
				return (1);
		}
		i++;
	}
	return (0);
}

int	token_needs_expansion(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == ENV_VAR || token->type == EXIT_STATUS)
		return (1);
	if (token->type == WORD && word_contains_variables(token->content))
		return (1);
	return (0);
}

void	mark_tokens_for_expansion(t_token **token_list)
{
	t_token	*current;

	if (!token_list || !*token_list)
		return ;
	current = *token_list;
	while (current)
	{
		current->needs_expansion = token_needs_expansion(current);
		current = current->next;
	}
}

/*
void	print_detected_variables(t_token **token_list)
{
	t_token	*current;
	int		count;

	if (!token_list || !*token_list)
		return ;
	count = 0;
	current = *token_list;
	printf("=== VARIABLES DÉTECTÉES ===\n");
	while (current)
	{
		if (current->needs_expansion)
		{
			printf("[%d] Type: %d, Content: \"%s\", Needs expansion: %s\n",
				count, current->type, current->content ? current->content : "NULL",
				current->needs_expansion ? "YES" : "NO");
			if (current->type == ENV_VAR)
				printf("    → Variable d'environnement: $%s\n", current->content);
			else if (current->type == EXIT_STATUS)
				printf("    → Exit status: $?\n");
			else if (current->type == WORD)
				printf("    → Word avec variables: %s\n", current->content);
		}
		current = current->next;
		count++;
	}
}
	*/