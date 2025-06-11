/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimal_token_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: [votre_nom] <[votre_email]>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:00:00 by [votre_nom]       #+#    #+#             */
/*   Updated: 2025/06/11 19:00:00 by [votre_nom]      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Includes nécessaires pour la tokenisation seulement
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libft/libft.h"

// Enums et structures directement copiés (pour éviter les dépendances)
typedef enum e_token_type
{
    DEFAULT = 0,
    WORD = 1,
    ASSIGNMENT = 2,
    QUOTE = 3,
    SINGLE_QUOTE = 4,
    DOUBLE_QUOTE = 5,
    REDIR = 6,
    REDIR_IN = 7,
    REDIR_OUT = 8,
    REDIR_APPEND = 9,
    HEREDOC = 10,
    OPERATOR = 11,
    PIPE = 12,
    AND = 13,
    OR = 14,
    SEMICOLON = 15,
    PARENTHESIS = 16,
    PAREN_OPEN = 17,
    PAREN_CLOSE = 18,
    SPECIAL_CARACTER = 19,
    ENV_VAR = 20,        // ← IMPORTANT : variable d'environnement
    EXIT_STATUS = 21,    // ← IMPORTANT : $?
    ESCAPE = 22,
    COMMENT = 23,
    T_NEWLINE = 24,
    T_EOF = 25,
    UNKNOWN = 26
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *content;
    struct s_token  *next;
    struct s_token  *prev;
} t_token;

// Déclarations des fonctions que vous avez déjà
t_token **tokenize_input(char *command);
void free_token_list(t_token **token_list);

// Test simple et direct
void test_variable_detection(void)
{
    printf("╔════════════════════════════════════════╗\n");
    printf("║     TEST MINIMAL DÉTECTION VARIABLES  ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    char *tests[] = {
        "echo $USER",
        "$HOME/file",
        "cmd $?",
        "echo $USER $HOME $?",
        NULL
    };
    
    for (int i = 0; tests[i] != NULL; i++)
    {
        printf("🧪 Test %d: %s\n", i + 1, tests[i]);
        printf("────────────────────────────\n");
        
        t_token **tokens = tokenize_input(tests[i]);
        if (!tokens || !*tokens)
        {
            printf("❌ Erreur tokenisation\n\n");
            continue;
        }
        
        t_token *current = *tokens;
        int token_num = 1;
        int variables_found = 0;
        
        while (current)
        {
            printf("[%d] \"%s\" (type=%d) ", token_num, current->content, current->type);
            
            if (current->type == ENV_VAR)
            {
                printf("← ✅ VARIABLE ENV\n");
                variables_found++;
            }
            else if (current->type == EXIT_STATUS)
            {
                printf("← ✅ EXIT STATUS\n");
                variables_found++;
            }
            else
            {
                printf("\n");
            }
            
            current = current->next;
            token_num++;
        }
        
        printf("📊 Variables détectées: %d\n", variables_found);
        printf("%s\n\n", variables_found > 0 ? "✅ DÉTECTION OK" : "❌ AUCUNE VARIABLE");
        
        free_token_list(tokens);
    }
}

int main(void)
{
    test_variable_detection();
    
    printf("🎯 CONCLUSION:\n");
    printf("Si vous voyez '✅ VARIABLE ENV' ou '✅ EXIT STATUS' ci-dessus,\n");
    printf("votre détection de variables fonctionne PARFAITEMENT !\n");
    
    return (0);
}