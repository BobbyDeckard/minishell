/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_debug_test.c                               :+:      :+:    :+:   */
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

// Types copiés
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

// Déclarations externes
t_token *create_token(void);
void define_special_caracter(char **command, t_token *token);
void free_token_list(t_token **token_list);
void link_token(t_token *token, t_token **token_list);

// Test 1: define_special_caracter seule
void test_define_special_caracter_isolated(void)
{
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║       TEST ISOLÉ define_special_caracter      ║\n");
    printf("╚═══════════════════════════════════════════════╝\n\n");
    
    // Test avec $USER
    char test_str[] = "$USER";  // Array local pour éviter les problèmes
    char *ptr = test_str;
    char *original_ptr = ptr;
    
    printf("🧪 Test: define_special_caracter avec \"%s\"\n", test_str);
    printf("─────────────────────────────────────────────\n");
    printf("Avant: ptr = %p, *ptr = '%c', string = \"%s\"\n", ptr, *ptr, ptr);
    
    t_token *token = create_token();
    if (!token)
    {
        printf("❌ Erreur: create_token() a échoué\n");
        return;
    }
    
    printf("Token créé, type initial = %d\n", token->type);
    
    // Appeler define_special_caracter
    define_special_caracter(&ptr, token);
    
    printf("\nAprès define_special_caracter:\n");
    printf("  - ptr = %p (moved %ld chars)\n", ptr, ptr - original_ptr);
    printf("  - *ptr = '%c'\n", *ptr ? *ptr : '\\0');
    printf("  - remaining string = \"%s\"\n", ptr);
    printf("  - token->type = %d\n", token->type);
    printf("  - token->content = \"%s\"\n", token->content ? token->content : "NULL");
    
    printf("\n📊 Résultat:\n");
    if (token->type == ENV_VAR && token->content && !strcmp(token->content, "USER"))
    {
        printf("✅ SUCCÈS: Variable correctement détectée!\n");
    }
    else
    {
        printf("❌ ÉCHEC: Problème dans define_special_caracter\n");
    }
    
    // Cleanup
    if (token->content)
        free(token->content);
    free(token);
    
    printf("\n");
}

// Test 2: define_special_caracter avec $?
void test_define_special_caracter_exit_status(void)
{
    printf("🧪 Test: define_special_caracter avec \"$?\"\n");
    printf("─────────────────────────────────────────────\n");
    
    char test_str[] = "$?";
    char *ptr = test_str;
    char *original_ptr = ptr;
    
    printf("Avant: ptr = %p, *ptr = '%c', string = \"%s\"\n", ptr, *ptr, ptr);
    
    t_token *token = create_token();
    define_special_caracter(&ptr, token);
    
    printf("Après: ptr moved %ld chars\n", ptr - original_ptr);
    printf("  - token->type = %d (should be 21 for EXIT_STATUS)\n", token->type);
    printf("  - token->content = \"%s\" (should be \"?\")\n", 
           token->content ? token->content : "NULL");
    
    if (token->type == EXIT_STATUS && token->content && !strcmp(token->content, "?"))
    {
        printf("✅ SUCCÈS: Exit status correctement détecté!\n");
    }
    else
    {
        printf("❌ ÉCHEC: Problème avec exit status\n");
    }
    
    if (token->content)
        free(token->content);
    free(token);
    
    printf("\n");
}

// Test 3: Simuler extract_token
void test_extract_token_simulation(void)
{
    printf("🧪 Test: Simulation extract_token\n");
    printf("─────────────────────────────────────────────\n");
    
    char test_str[] = "echo $USER";
    char *ptr = test_str;
    
    printf("Input: \"%s\"\n", test_str);
    
    // Simuler le traitement caractère par caractère
    int step = 1;
    while (*ptr)
    {
        printf("\nStep %d: char='%c' at position %ld\n", step, *ptr, ptr - test_str);
        
        if (*ptr == ' ')
        {
            printf("  → Space, skipping\n");
            ptr++;
        }
        else if (*ptr == '$')
        {
            printf("  → Found $, calling define_special_caracter\n");
            char *before = ptr;
            
            t_token *token = create_token();
            define_special_caracter(&ptr, token);
            
            printf("  → Pointer advanced by %ld chars\n", ptr - before);
            printf("  → Token type: %d, content: \"%s\"\n", 
                   token->type, token->content ? token->content : "NULL");
            
            if (token->content)
                free(token->content);
            free(token);
        }
        else
        {
            printf("  → Regular char, skipping for this test\n");
            ptr++;
        }
        
        step++;
        if (step > 10) // Safety
        {
            printf("⚠️ Stopping after 10 steps\n");
            break;
        }
    }
}

// Test 4: Test de corruption mémoire
void test_memory_corruption_check(void)
{
    printf("🧪 Test: Vérification corruption mémoire\n");
    printf("─────────────────────────────────────────────\n");
    
    char test_str[] = "$USER";
    char backup[10];
    strcpy(backup, test_str);
    
    char *ptr = test_str;
    
    printf("Avant: string=\"%s\", backup=\"%s\"\n", test_str, backup);
    
    t_token *token = create_token();
    define_special_caracter(&ptr, token);
    
    printf("Après: string=\"%s\", backup=\"%s\"\n", test_str, backup);
    
    if (strcmp(test_str, backup) == 0)
    {
        printf("✅ Pas de corruption: string originale intacte\n");
    }
    else
    {
        printf("❌ CORRUPTION: string originale modifiée!\n");
    }
    
    if (token->content)
        free(token->content);
    free(token);
}

int main(void)
{
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║            DIAGNOSTIC COMPLET                ║\n");
    printf("╚═══════════════════════════════════════════════╝\n\n");
    
    test_define_special_caracter_isolated();
    test_define_special_caracter_exit_status();
    test_extract_token_simulation();
    test_memory_corruption_check();
    
    printf("🎯 CONCLUSION:\n");
    printf("Ce test va révéler exactement où est le problème!\n");
    
    return (0);
}