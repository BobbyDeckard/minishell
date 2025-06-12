/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:37:08 by pitran            #+#    #+#             */
/*   Updated: 2025/06/12 13:15:57 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

t_token	*tokenize_special_caracter(char **command, t_token *token)
{
	int	len;
	int	i;

	// Calculer la bonne longueur selon le type
	if (**command == '$')
	{
		if ((*command)[1] == '?')
			len = 2; // $?
		else if (is_command_char((*command)[1]))
		{
			// Calculer la longueur du nom de variable
			len = 1; // pour le $
			i = 1;
			while ((*command)[i] && is_command_char((*command)[i]))
			{
				len++;
				i++;
			}
		}
		else
			len = 1; // juste $
	}
	else
		len = 1; // autres caractères spéciaux
	
	define_special_caracter(command, token);
	
	token->content = (char *)malloc(sizeof(char) * (len + 1));
	if (!token->content)
		return (NULL);
	
	// Extraire le contenu selon le type
	if (token->type == ENV_VAR && len > 1)
	{
		// Pour les variables d'environnement, on stocke le nom sans le $
		i = 0;
		while (i < len - 1 && is_command_char((*command)[i + 1]))
		{
			token->content[i] = (*command)[i + 1];
			i++;
		}
		token->content[i] = '\0';
	}
	else if (token->type == EXIT_STATUS)
	{
		// Pour $?, on stocke juste "?"
		token->content[0] = '?';
		token->content[1] = '\0';
	}
	else
	{
		// Pour les autres cas, on stocke le caractère tel quel
		token->content[0] = **command;
		token->content[1] = '\0';
	}
	
	*command += len;
	return (token);
}

/*
** Détection et extraction des variables shell à expandre
** Gère : $USER, $? etc.
*/

void	define_special_caracter(char **command, t_token *token)
{
	char	*start;
	int		len;

	printf("    DEBUG: define_special_caracter called\n");
	printf("    DEBUG: **command = '%c', address = %p\n", **command, *command);
	
	if (**command != '$')
	{
		printf("    DEBUG: Not a $ character, returning\n");
		return ;
	}
	
	if ((*command)[1] == '?')
	{
		printf("    DEBUG: Found $? pattern\n");
		token->type = EXIT_STATUS;
		token->content = ft_strdup("?");
		(*command) += 2;
		printf("    DEBUG: Advanced by 2, new pos = %p\n", *command);
	}
	else if (ft_isalpha((*command)[1]) || (*command)[1] == '_')
	{
		printf("    DEBUG: Found $VAR pattern\n");
		token->type = ENV_VAR;
		(*command)++;
		start = *command;
		printf("    DEBUG: start = %p ('%c')\n", start, *start);
		
		while (ft_isalnum(**command) || **command == '_')
		{
			printf("    DEBUG: Reading '%c'\n", **command);
			(*command)++;
		}
		
		len = *command - start;
		printf("    DEBUG: Variable length = %d\n", len);
		printf("    DEBUG: Final position = %p\n", *command);
		
		token->content = ft_substr(start, 0, len);
		printf("    DEBUG: Extracted content = '%s'\n", token->content);
	}
	else
	{
		printf("    DEBUG: Unknown $ pattern\n");
		token->type = UNKNOWN;
		token->content = ft_strdup("$");
		(*command)++;
	}
}

void	define_token_type(char **command, t_token *token)
{
	if (**command == '$')
		define_special_caracter(command, token);
	else if (**command == '\\')
	{
		token->type = ESCAPE;
		(*command)++;
	}
	else if (**command == '#')
	{
		token->type = COMMENT;
		(*command)++;
	}
	else if (**command == '\n')
	{
		token->type = T_NEWLINE;
		(*command)++;
	}
	else if (**command == '\0')
	{
		token->type = T_EOF;
	}
	else
	{
		token->type = WORD;
		(*command)++;
	}
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