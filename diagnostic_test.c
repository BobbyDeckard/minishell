/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diagnostic_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: [votre_nom] <[votre_email]>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:00:00 by [votre_nom]       #+#    #+#             */
/*   Updated: 2025/06/11 19:00:00 by [votre_nom]      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Includes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libft/libft.h"

// Types (copiés pour éviter dépendances)
typedef enum e_token_type
{
    DEFAULT = 0, WORD = 1, ASSIGNMENT = 2, QUOTE = 3, SINGLE_QUOTE = 4,
    DOUBLE_QUOTE = 5, REDIR = 6, REDIR_IN = 7, REDIR_OUT = 8, REDIR_APPEND = 9,
    HEREDOC = 10, OPERATOR = 11, PIPE = 12, AND = 13, OR = 14, SEMICOLON = 15,
    PARENTHESIS = 16, PAREN_OPEN = 17, PAREN_CLOSE = 18, SPECIAL_CARACTER = 19,
    ENV_VAR = 20, EXIT_STATUS = 21, ESCAPE = 22, COMMENT = 23,
    T_NEWLINE = 24, T_EOF = 25, UNKNOWN = 26
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *content;
    struct s_token  *next;
    struct s_token  *prev;
} t_token;

// Déclarations
t_token **tokenize_input(char *command);
void free_token_list(t_token **token_list);

// Fonction pour afficher le nom du type
const char *get_type_name(t_token_type type)
{
    switch(type)
    {
        case WORD: return "WORD";
        case ENV_VAR: return "ENV_VAR";
        case EXIT_STATUS: return "EXIT_STATUS";
        case SINGLE_QUOTE: return "SINGLE_QUOTE";
        case DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case PIPE: return "PIPE";
        case SPECIAL_CARACTER: return "SPECIAL_CARACTER";
        default: return "OTHER";
    }
}

// Test diagnostic complet
void diagnostic_test(const char *input)
{
    printf("\n🔍 DIAGNOSTIC: %s\n", input);
    printf("═══════════════════════════════════════\n");
    
    t_token **tokens = tokenize_input((char *)input);
    if (!tokens || !*tokens)
    {
        printf("❌ ERREUR: tokenize_input retourne NULL\n");
        return;
    }
    
    t_token *current = *tokens;
    int token_count = 0;
    int variable_count = 0;
    
    printf("Tokens générés:\n");
    while (current)
    {
        token_count++;
        printf("  [%d] \"%s\" → type=%d (%s)", 
               token_count, 
               current->content ? current->content : "NULL", 
               current->type,
               get_type_name(current->type));
        
        if (current->type == ENV_VAR)
        {
            printf(" ✅ VARIABLE!");
            variable_count++;
        }
        else if (current->type == EXIT_STATUS)
        {
            printf(" ✅ EXIT_STATUS!");
            variable_count++;
        }
        
        printf("\n");
        current = current->next;
    }
    
    printf("\n📊 BILAN:\n");
    printf("  - Total tokens: %d\n", token_count);
    printf("  - Variables détectées: %d\n", variable_count);
    printf("  - Statut: %s\n", variable_count > 0 ? "✅ DÉTECTION OK" : "❌ AUCUNE DÉTECTION");
    
    free_token_list(tokens);
}

int main(void)
{
    
}