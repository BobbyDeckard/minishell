/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:37:08 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:41:36 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

t_token	*tokenize_special_caracter(char **command, t_token *token)
{
	int	len;
	int	i;

	len = 1;
	define_special_caracter(command, token);
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		return (NULL);
	i = 0;
	token->content[i] = *(command)[i];
	token->content[++i] = '\0';
	*command += len;
	return (token);
}

void	define_special_caracter(char **command, t_token *token)
{
	if (**command == '$')
	{
		if (is_command_char((*command)[1]))
			token->type = ENV_VAR;
		else if ((*command)[1] == '?')
			token->type = EXIT_STATUS;
		else
			token->type = UNKNOWN;
	}
	else if (**command == '\\')
		token->type = ESCAPE;
	else if (**command == '#')
		token->type = COMMENT;
	else if (**command == '\n')
		token->type = T_NEWLINE;
	else if (**command == '\0')
		token->type = T_EOF;
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
