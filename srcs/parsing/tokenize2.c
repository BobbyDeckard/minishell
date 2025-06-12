/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:37:08 by pitran            #+#    #+#             */
/*   Updated: 2025/06/12 15:22:43 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/*
** ========================================================================
** FONCTION PRINCIPALE : tokenize_special_caracter
** ========================================================================
** Détecte et stocke les variables d'environnement pour l'expander
*/
t_token	*tokenize_special_caracter(char **command, t_token *token)
{
	char	*start;
	int		len;

	if (**command != '$')
	{
		token->type = UNKNOWN;
		token->content = malloc(2);
		if (!token->content)
			return (NULL);
		token->content[0] = **command;
		token->content[1] = '\0';
		(*command)++;
		return (token);
	}
	if ((*command)[1] == '?')
	{
		token->type = EXIT_STATUS;
		token->content = ft_strdup("?");
		*command += 2;
	}
	else if (ft_isalpha((*command)[1]) || (*command)[1] == '_')
	{
		token->type = ENV_VAR;
		(*command)++;
		start = *command;
		len = 0;
		while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
			len++;
		token->content = ft_substr(start, 0, len);
		*command += len;
	}
	else
	{
		token->type = WORD;
		token->content = ft_strdup("$");
		(*command)++;
	}
	return (token);
}

void	define_special_caracter(char **command, t_token *token)
{
	if (**command != '$')
	{
		token->type = UNKNOWN;
		return ;
	}
	if ((*command)[1] == '?')
		token->type = EXIT_STATUS;
	else if (ft_isalpha((*command)[1]) || (*command)[1] == '_')
		token->type = ENV_VAR;
	else
		token->type = UNKNOWN;
}

void	define_double_redirection(char **command, t_token *token, int *len)
{
	if (**command == '>' && (*command)[1] == '>')
		token->type = REDIR_APPEND;
	else if (**command == '<' && (*command)[1] == '<')
		token->type = HEREDOC;
	else
		token->type = UNKNOWN;
	*(len) = 2;
}

void	define_bonus_operator(char **command, t_token *token, int *len)
{
	if (**command == '&' && (*command)[1] == '&')
		token->type = AND;
	else if (**command == '|' && (*command)[1] == '|')
		token->type = OR;
	else
		token->type = UNKNOWN;
	*(len) = 2;
}