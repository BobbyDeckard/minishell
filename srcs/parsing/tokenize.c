/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:36:36 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:41:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

t_token	*tokenize_word(char **command, t_token *token)
{
	int		len;
	int		i;

	token->type = WORD;
	len = find_word_len(*command);
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		return (NULL);
	i = 0;
	while (is_command_char((*command)[i]))
	{
		token->content[i] = (*command)[i];
		i++;
	}
	token->content[i] = '\0';
	*command += len;
	return (token);
}

t_token	*tokenize_quote(char **command, t_token *token)
{
	int	len;
	int	i;

	len = 1;
	if (**command == 39)
		token->type = SINGLE_QUOTE;
	else if (**command == 34)
		token->type = DOUBLE_QUOTE;
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		return (NULL);
	i = 0;
	token->content[i] = (*command)[i];
	token->content[++i] = '\0';
	*command += len;
	return (token);
}

t_token	*tokenize_redir(char **command, t_token *token)
{
	int	len;
	int	i;

	len = 1;
	if (**command == '>')
		token->type = REDIR_OUT;
	else if (**command == '<')
		token->type = REDIR_IN;
	if (is_redirection((*command)[1]))
		define_double_redirection(command, token, &len);
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		return (NULL);
	i = 0;
	while (is_redirection((*command)[i]) && i < len)
	{
		token->content[i] = (*command)[i];
		i++;
	}
	token->content[i] = '\0';
	*command += len;
	return (token);
}

t_token	*tokenize_operator(char **command, t_token *token)
{
	int	len;
	int	i;

	len = 1;
	if (**command == '|')
		token->type = PIPE;
	else if (**command == ';')
		token->type = SEMICOLON;
	if (is_operator((*command)[1]))
		define_bonus_operator(command, token, &len);
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (is_operator((*command)[i]) && i < len)
	{
		token->content[i] = (*command)[i];
		i++;
	}
	token->content[i] = '\0';
	*command += len;
	return (token);
}

t_token	*tokenize_parenthesis(char **command, t_token *token)
{
	int	len;
	int	i;

	len = 1;
	if (**command == '(')
		token->type = PAREN_OPEN;
	else if (**command == ')')
		token->type = PAREN_CLOSE;
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		return (NULL);
	i = 0;
	token->content[i] = (*command)[i];
	token->content[++i] = '\0';
	*command += len;
	return (token);
}
