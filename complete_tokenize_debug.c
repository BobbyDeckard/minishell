/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_tokenize_debug.c                           :+:      :+:    :+:   */
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
t_token_type get_token_type(char c);
t_token **extract_token(char **command, t_token_type type, t_token **token_list);
t_token *create_token(void);
void link_token(t_token *token, t_token **token_list);
void free_token_list(t_token **token_list);

// Fonction utilitaire pour afficher le nom du type
const char *get_type_name(t_token_type type)
{
    switch(type)
    {
        case DEFAULT: return "DEFAULT";
        case WORD: return "WORD";
        case ENV_VAR: return "ENV_VAR";
        case EXIT_STATUS: return "EXIT_STATUS";
        case SINGLE_QUOTE: return "SINGLE_QUOTE";
        case DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case SPECIAL_CARACTER: return "SPECIAL_CARACTER";
        case PIPE: return "PIPE";
        default: return "OTHER";
    }
}

// Fonction pour compter les tokens dans la liste
int count_tokens_in_list(t_token **token_list)
{
    int count = 0;
    if (!token_list || !*token_list)
        return 0;
    
    t_token *current = *token_list;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

// Fonction pour afficher TOUS les tokens de la liste
void print_all_tokens_in_list(t_token **token_list, const char *label)
{
    printf("  📋 %s:\n", label);
    if (!token_list || !*token_list)
    {
        printf("    (empty list)\n");
        return;
    }
    
    t_token *current = *token_list;
    int index = 1;
    
    while (current)
    {
        printf("    [%d] \"%s\" (type=%d %s) addr=%p\n", 
               index, 
               current->content ? current->content : "NULL",
               current->type,
               get_type_name(current->type),
               current);
        current = current->next;
        index++;
    }
}

// Test de extract_token en isolation
void test_extract_token_isolated(void)
{
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║           TEST EXTRACT_TOKEN ISOLÉ            ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    // Test 1: Créer une liste vide et ajouter un token WORD
    printf("🧪 Test 1: extract_token avec WORD\n");
    printf("─────────────────────────────────────────────\n");
    
    char test1[] = "echo rest";
    char *ptr1 = test1;
    
    t_token **token_list1 = malloc(sizeof(t_token *));
    *token_list1 = NULL;
    
    printf("Avant extract_token:\n");
    printf("  Input: \"%s\"\n", test1);
    printf("  Pointer: %p ('%c')\n", ptr1, *ptr1);
    print_all_tokens_in_list(token_list1, "Token list");
    
    t_token_type type1 = get_token_type(*ptr1);
    printf("  get_token_type('%c') = %d (%s)\n", *ptr1, type1, get_type_name(type1));
    
    t_token **result1 = extract_token(&ptr1, type1, token_list1);
    
    printf("\nAprès extract_token:\n");
    printf("  Pointer advanced to: %p ('%c')\n", ptr1, *ptr1 ? *ptr1 : '\\0');
    printf("  Remaining string: \"%s\"\n", ptr1);
    printf("  extract_token returned: %p\n", result1);
    printf("  Same as input list? %s\n", result1 == token_list1 ? "YES" : "NO");
    
    print_all_tokens_in_list(result1, "Result token list");
    
    free_token_list(result1);
    
    printf("\n");
    
    // Test 2: extract_token avec SPECIAL_CARACTER
    printf("🧪 Test 2: extract_token avec SPECIAL_CARACTER\n");
    printf("─────────────────────────────────────────────\n");
    
    char test2[] = "$USER";
    char *ptr2 = test2;
    
    t_token **token_list2 = malloc(sizeof(t_token *));
    *token_list2 = NULL;
    
    printf("Avant extract_token:\n");
    printf("  Input: \"%s\"\n", test2);
    printf("  Pointer: %p ('%c')\n", ptr2, *ptr2);
    print_all_tokens_in_list(token_list2, "Token list");
    
    t_token_type type2 = get_token_type(*ptr2);
    printf("  get_token_type('%c') = %d (%s)\n", *ptr2, type2, get_type_name(type2));
    
    t_token **result2 = extract_token(&ptr2, type2, token_list2);
    
    printf("\nAprès extract_token:\n");
    printf("  Pointer advanced to: %p ('%c')\n", ptr2, *ptr2 ? *ptr2 : '\\0');
    printf("  Remaining string: \"%s\"\n", ptr2);
    printf("  extract_token returned: %p\n", result2);
    
    print_all_tokens_in_list(result2, "Result token list");
    
    free_token_list(result2);
    
    printf("\n");
}

// Test de tokenize_input complet avec debug
void test_tokenize_input_with_debug(const char *input)
{
    printf("🧪 Test tokenize_input: \"%s\"\n", input);
    printf("═══════════════════════════════════════════════\n");
    
    // Copier l'input pour éviter les modifications
    char *command = malloc(strlen(input) + 1);
    strcpy(command, input);
    char *original_command = command;
    
    t_token **token_list = malloc(sizeof(t_token *));
    if (!token_list)
    {
        printf("❌ Erreur malloc token_list\n");
        free(command);
        return;
    }
    *token_list = NULL;
    
    printf("Début tokenize_input simulation:\n");
    printf("  Input: \"%s\"\n", input);
    printf("  Command pointer: %p\n", command);
    
    int step = 1;
    while (*command)
    {
        printf("\nStep %d: char='%c' (pos=%ld)\n", step, *command, command - original_command);
        
        t_token_type type = get_token_type(*command);
        printf("  → get_token_type('%c') = %d (%s)\n", *command, type, get_type_name(type));
        
        if (type == DEFAULT)
        {
            printf("  → DEFAULT type, skipping char\n");
            command++;
            continue;
        }
        
        printf("  → Calling extract_token...\n");
        printf("    Before: command=%p ('%c'), list has %d tokens\n", 
               command, *command, count_tokens_in_list(token_list));
        
        char *before_ptr = command;
        t_token **result = extract_token(&command, type, token_list);
        
        printf("    After: command=%p ('%c'), advanced by %ld chars\n", 
               command, *command ? *command : '\\0', command - before_ptr);
        printf("    extract_token returned: %p (same as input? %s)\n", 
               result, result == token_list ? "YES" : "NO");
        
        if (!result)
        {
            printf("  ❌ extract_token returned NULL!\n");
            break;
        }
        
        token_list = result;
        printf("    Updated token_list now has %d tokens\n", count_tokens_in_list(token_list));
        
        // Afficher le dernier token ajouté
        if (*token_list)
        {
            t_token *current = *token_list;
            while (current->next) current = current->next; // Aller au dernier
            printf("    Latest token: \"%s\" (type=%d %s)\n", 
                   current->content ? current->content : "NULL", 
                   current->type, get_type_name(current->type));
        }
        
        step++;
        if (step > 20) // Sécurité
        {
            printf("⚠️ Stopping after 20 steps\n");
            break;
        }
    }
    
    printf("\n🏁 Tokenization complete!\n");
    printf("Final token count: %d\n", count_tokens_in_list(token_list));
    
    print_all_tokens_in_list(token_list, "Final token list");
    
    // Cleanup
    free_token_list(token_list);
    free(original_command);
    
    printf("\n");
}

// Test des fonctions de base
void test_basic_functions(void)
{
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║              TEST FONCTIONS DE BASE           ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    // Test create_token
    printf("🧪 Test create_token:\n");
    t_token *token = create_token();
    if (token)
    {
        printf("✅ create_token success: type=%d, content=%p, next=%p, prev=%p\n",
               token->type, token->content, token->next, token->prev);
        free(token);
    }
    else
    {
        printf("❌ create_token failed\n");
    }
    
    // Test get_token_type
    printf("\n🧪 Test get_token_type:\n");
    char test_chars[] = {'e', ' ', '$', '?', '|', '"', '\''};
    int num_chars = sizeof(test_chars) / sizeof(test_chars[0]);
    
    for (int i = 0; i < num_chars; i++)
    {
        t_token_type type = get_token_type(test_chars[i]);
        printf("  get_token_type('%c') = %d (%s)\n", 
               test_chars[i], type, get_type_name(type));
    }
    
    printf("\n");
}

int main(void)
{
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║         DEBUG COMPLET TOKENIZE_INPUT          ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    test_basic_functions();
    test_extract_token_isolated();
    
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║           TESTS TOKENIZE_INPUT COMPLETS       ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    test_tokenize_input_with_debug("echo");
    test_tokenize_input_with_debug("$USER");
    test_tokenize_input_with_debug("echo $USER");
    test_tokenize_input_with_debug("$? test");
    
    printf("🎯 CONCLUSION:\n");
    printf("Ce debug va révéler exactement où le problème se situe\n");
    printf("dans tokenize_input et extract_token!\n");
    
    return (0);
}