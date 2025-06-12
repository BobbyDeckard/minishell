/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   improved_debug_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: [votre_nom] <[votre_email]>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:00:00 by [votre_nom]       #+#    #+#             */
/*   Updated: 2025/06/11 19:00:00 by [votre_nom]      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libft/libft.h"

// Types
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
    int             needs_expansion;
} t_token;

// Déclarations
t_token **tokenize_input(char *command);
void free_token_list(t_token **token_list);

const char *get_type_name(t_token_type type)
{
    switch(type)
    {
        case WORD: return "WORD";
        case ENV_VAR: return "ENV_VAR";
        case EXIT_STATUS: return "EXIT_STATUS";
        case DEFAULT: return "DEFAULT";
        case SPECIAL_CARACTER: return "SPECIAL_CARACTER";
        default: return "OTHER";
    }
}

// Fonction de debug VRAIMENT complète
void debug_complete_token_list(t_token **token_list, const char *input)
{
    printf("\n🔍 ANALYSE COMPLÈTE pour \"%s\"\n", input);
    printf("═══════════════════════════════════════════\n");
    
    if (!token_list) {
        printf("❌ token_list pointer is NULL\n");
        return;
    }
    
    if (!*token_list) {
        printf("❌ *token_list is NULL (empty list)\n");
        return;
    }
    
    printf("✅ token_list = %p, *token_list = %p\n", token_list, *token_list);
    
    t_token *current = *token_list;
    int count = 0;
    
    while (current) {
        count++;
        printf("\n[Token %d]\n", count);
        printf("  Address: %p\n", current);
        printf("  Type: %d (%s)\n", current->type, get_type_name(current->type));
        printf("  Content: \"%s\"\n", current->content ? current->content : "NULL");
        printf("  needs_expansion: %d\n", current->needs_expansion);
        printf("  Next: %p\n", current->next);
        printf("  Prev: %p\n", current->prev);
        
        // Vérification de cohérence
        if (current->next && current->next->prev != current) {
            printf("  ⚠️  WARNING: next->prev inconsistency!\n");
        }
        
        current = current->next;
        
        if (count > 10) {
            printf("  ... (stopping at 10 for safety)\n");
            break;
        }
    }
    
    printf("\n📊 RÉSUMÉ:\n");
    printf("  Total tokens: %d\n", count);
    
    // Test de parcours inverse
    printf("\n🔄 Test parcours inverse:\n");
    current = *token_list;
    while (current && current->next) {
        current = current->next;
    }
    
    int reverse_count = 0;
    while (current) {
        reverse_count++;
        printf("  [%d] \"%s\" (type=%s)\n", 
               reverse_count, 
               current->content ? current->content : "NULL",
               get_type_name(current->type));
        current = current->prev;
        
        if (reverse_count > 10) break;
    }
    
    if (count != reverse_count) {
        printf("⚠️  WARNING: Forward count (%d) != Reverse count (%d)\n", 
               count, reverse_count);
    } else {
        printf("✅ Liste doublement liée cohérente\n");
    }
}

int main(void)
{
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║           TEST DEBUG AMÉLIORÉ                 ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    // Test des cas problématiques
    char *tests[] = {
        "echo",
        "$USER", 
        "echo $USER",
        "$USER $HOME",
        NULL
    };
    
    for (int i = 0; tests[i]; i++) {
        printf("\n🧪 Test %d: %s\n", i + 1, tests[i]);
        
        t_token **tokens = tokenize_input(tests[i]);
        debug_complete_token_list(tokens, tests[i]);
        
        if (tokens) {
            free_token_list(tokens);
        }
        
        printf("\n" "─────────────────────────────────────────────\n");
    }
    
    printf("\n🎯 CONCLUSION:\n");
    printf("Ce test va révéler s'il y a vraiment un problème\n");
    printf("dans la création des tokens ou juste dans notre debug!\n");
    
    return (0);
}