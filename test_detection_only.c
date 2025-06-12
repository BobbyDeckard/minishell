/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_detection_only.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by [votre_nom]       #+#    #+#             */
/*   Updated: 2025/06/12 15:27:44 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Types simplifiés
typedef enum e_token_type
{
    DEFAULT = 0, WORD = 1, ENV_VAR = 20, EXIT_STATUS = 21, 
    SPECIAL_CARACTER = 19, UNKNOWN = 26
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *content;
    int             needs_expansion;
    struct s_token  *next;
    struct s_token  *prev;
} t_token;

// Déclarations
t_token **tokenize_input(char *command);
void free_token_list(t_token **token_list);
void print_detected_variables(t_token **token_list);

const char *get_type_name(t_token_type type)
{
    switch(type)
    {
        case WORD: return "WORD";
        case ENV_VAR: return "ENV_VAR";
        case EXIT_STATUS: return "EXIT_STATUS";
        case SPECIAL_CARACTER: return "SPECIAL_CARACTER";
        case UNKNOWN: return "UNKNOWN";
        default: return "OTHER";
    }
}

void test_detection(const char *input)
{
    printf("\n🧪 TEST: \"%s\"\n", input);
    printf("═══════════════════════════════════════\n");
    
    t_token **tokens = tokenize_input((char *)input);
    if (!tokens || !*tokens)
    {
        printf("❌ ERREUR: Tokenisation échouée\n");
        return;
    }
    
    // Afficher tous les tokens
    t_token *current = *tokens;
    int count = 1;
    printf("Tokens générés:\n");
    while (current)
    {
        printf("  [%d] \"%s\" → %s", 
               count,
               current->content ? current->content : "NULL",
               get_type_name(current->type));
        
        if (current->needs_expansion)
            printf(" ✅ EXPANSION NÉCESSAIRE");
        
        printf("\n");
        current = current->next;
        count++;
    }
    
    // Résumé des variables détectées
    printf("\n📊 RÉSUMÉ DES VARIABLES:\n");
    current = *tokens;
    int env_vars = 0, exit_status = 0, words_with_vars = 0;
    
    while (current)
    {
        if (current->needs_expansion)
        {
            if (current->type == ENV_VAR)
                env_vars++;
            else if (current->type == EXIT_STATUS)
                exit_status++;
            else if (current->type == WORD)
                words_with_vars++;
        }
        current = current->next;
    }
    
    printf("  - Variables d'environnement: %d\n", env_vars);
    printf("  - Exit status ($?): %d\n", exit_status);
    printf("  - Words avec variables: %d\n", words_with_vars);
    
    free_token_list(tokens);
}

int main(void)
{
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║           TEST DÉTECTION VARIABLES           ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    // Tests de base
    test_detection("echo $USER");
    test_detection("$USER");
    test_detection("echo $?");
    test_detection("$?");
    test_detection("echo $USER $HOME");
    test_detection("echo hello$USER");
    test_detection("echo $USER$HOME");
    test_detection("$USER$?");
    test_detection("echo '$USER'");  // Dans quotes simples
    test_detection("echo \"$USER\""); // Dans quotes doubles
    
    // Tests edge cases
    test_detection("echo $");         // $ seul
    test_detection("echo $123");      // $ + chiffre
    test_detection("echo $$");        // $$
    test_detection("echo $USER_VAR"); // avec underscore
    
    printf("\n🎯 POUR VOTRE BINÔME:\n");
    printf("Les tokens avec needs_expansion = 1 doivent être expandés\n");
    printf("Les types à traiter:\n");
    printf("  - ENV_VAR: contient le nom de la variable (sans $)\n");
    printf("  - EXIT_STATUS: contient \"?\"\n");
    printf("  - WORD: contient du texte avec des $VAR à l'intérieur\n");
    
    return (0);
}