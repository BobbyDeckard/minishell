/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_extract_token.c                               :+:      :+:    :+:   */
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

// Types (copiés)
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

// Déclarations de vos fonctions
t_token_type get_token_type(char c);
t_token **extract_token(char **command, t_token_type type, t_token **token_list);
void free_token_list(t_token **token_list);

// Test de diagnostic avec traces
void debug_tokenize_step_by_step(char *input)
{
    printf("\n🔍 DEBUG STEP-BY-STEP: \"%s\"\n", input);
    printf("═══════════════════════════════════════════\n");
    
    char *command = input;
    t_token **token_list = malloc(sizeof(t_token *));
    *token_list = NULL;
    
    int step = 0;
    
    while (*command)
    {
        step++;
        printf("STEP %d: char='%c' (pos=%ld)\n", step, *command, command - input);
        
        t_token_type type = get_token_type(*command);
        printf("  → get_token_type('%c') = %d\n", *command, type);
        
        if (type == DEFAULT)
        {
            printf("  → DEFAULT type, skipping...\n");
            command++;
            continue;
        }
        
        char *before = command;
        printf("  → Calling extract_token...\n");
        
        t_token **result = extract_token(&command, type, token_list);
        
        printf("  → extract_token returned, command advanced by %ld chars\n", 
               command - before);
        printf("  → New position: '%c' (remaining: \"%s\")\n", 
               *command ? *command : '\0', command);
        
        if (!result)
        {
            printf("  ❌ extract_token failed!\n");
            free(token_list);
            return;
        }
        
        token_list = result;
        
        // Afficher le token créé
        if (*token_list)
        {
            t_token *current = *token_list;
            while (current->next) current = current->next; // Aller au dernier
            printf("  ✅ Token created: \"%s\" (type=%d)\n", 
                   current->content ? current->content : "NULL", current->type);
        }
        
        printf("\n");
        
        // Sécurité anti-boucle infinie
        if (step > 20)
        {
            printf("⚠️ Stopping after 20 steps to avoid infinite loop\n");
            break;
        }
    }
    
    printf("🏁 Tokenization finished after %d steps\n", step);
    
    // Afficher tous les tokens finaux
    printf("\n📋 FINAL TOKENS:\n");
    if (*token_list)
    {
        t_token *current = *token_list;
        int count = 1;
        while (current)
        {
            printf("  [%d] \"%s\" (type=%d)\n", 
                   count++, 
                   current->content ? current->content : "NULL", 
                   current->type);
            current = current->next;
        }
    }
    else
    {
        printf("  (no tokens)\n");
    }
    
    free_token_list(token_list);
}

int main(void)
{
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║        DEBUG EXTRACT_TOKEN                  ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    debug_tokenize_step_by_step("echo $USER");
    debug_tokenize_step_by_step("$USER");
    
    return (0);
}