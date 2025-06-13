/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_type_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:51:19 by pitran            #+#    #+#             */
/*   Updated: 2025/06/13 15:35:19 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     operator_syntax_is_valid(t_token **token_list)
{
        t_token *current;
        t_token *prev;

        current = *token_list;
        prev = NULL;
        while (current)
        {
                if (current->type == PIPE || current->type == AND || current->type == OR)
                {
                        // AJOUT: Vérifier qu'il y a un token AVANT (pas de pipe en début)
                        if (!prev)
                        {
                                printf("minishell: syntax error near unexpected token `%s'\n",
                                        current->content);
                                return (0);
                        }

                        // AJOUT: Vérifier que le token précédent est valide
                        if (prev->type != WORD && prev->type != PAREN_CLOSE)
                        {
                                printf("minishell: syntax error near unexpected token `%s'\n",
                                        current->content);
                                return (0);
                        }

                        // AJOUT: Vérifier qu'il y a un token APRÈS (pas de pipe en fin)
                        if (!current->next)
                        {
                                printf("minishell: syntax error near unexpected token `newline'\n");
                                return (0);
                        }

                        // AJOUT: Vérifier que le token suivant est valide
                        if (current->next->type != WORD && current->next->type != PAREN_OPEN)
                        {
                                printf("minishell: syntax error near unexpected token `%s'\n",
                                        current->content);
                                return (0);
                        }
                }
                prev = current;
                current = current->next;
        }
        return (1);
}

int     redir_syntax_is_valid(t_token **token_list)
{
        t_token *current;

        current = *token_list;
        while (current)
        {
                if (current->type == REDIR_IN || current->type == REDIR_OUT
                        || current->type == REDIR_APPEND || current->type == HEREDOC)
                {
                        // AJOUT: Vérifier qu'il y a un token après la redirection
                        if (!current->next)
                        {
                                printf("minishell: syntax error near unexpected token `newline'\n");
                                return (0);
                        }

                        // AJOUT: Vérifier que c'est un nom de fichier (WORD)
                        if (current->next->type != WORD)
                        {
                                printf("minishell: syntax error near unexpected token `%s'\n",
                                        current->next->content ? current->next->content : "newline");
                                return (0);
                        }
                }
                current = current->next;
        }
        return (1);
}

int     quote_syntax_is_valid(t_token **token_list)
{
        t_token *current;
        int             single_open;
        int             double_open;

        current = *token_list;
        single_open = 0;
        double_open = 0;

        while (current)
        {
                if (current->type == SINGLE_QUOTE)
                        single_open = !single_open;  // Toggle: 0->1->0
                else if (current->type == DOUBLE_QUOTE)
                        double_open = !double_open;  // Toggle: 0->1->0
                current = current->next;
        }

        // AJOUT: Si des guillemets restent ouverts, c'est une erreur
        if (single_open)
        {
                printf("minishell: syntax error: unterminated quoted string\n");
                return (0);
        }
        if (double_open)
        {
                printf("minishell: syntax error: unterminated quoted string\n");
                return (0);
        }
        return (1);
}
