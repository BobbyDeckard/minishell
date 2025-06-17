/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_unit_tests.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:00:00 by test              #+#    #+#             */
/*   Updated: 2025/06/13 18:13:59 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"
#include <assert.h>

/* Variables globales pour les tests */
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

/* Couleurs pour l'affichage */
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

/* ========================================================================== */
/* MACROS POUR LES TESTS                                                     */
/* ========================================================================== */

#define TEST_START(name) \
    printf(CYAN "=== Testing: %s ===" RESET "\n", name); \
    g_tests_run++;

#define ASSERT_TRUE(condition, message) \
    if (condition) { \
        printf(GREEN "✓ PASS" RESET ": %s\n", message); \
        g_tests_passed++; \
    } else { \
        printf(RED "✗ FAIL" RESET ": %s\n", message); \
        g_tests_failed++; \
    }

#define ASSERT_FALSE(condition, message) \
    ASSERT_TRUE(!(condition), message)

#define ASSERT_NOT_NULL(ptr, message) \
    ASSERT_TRUE((ptr) != NULL, message)

#define ASSERT_NULL(ptr, message) \
    ASSERT_TRUE((ptr) == NULL, message)

#define ASSERT_EQUAL(a, b, message) \
    ASSERT_TRUE((a) == (b), message)

#define ASSERT_STR_EQUAL(a, b, message) \
    ASSERT_TRUE(ft_strncmp((a), (b), ft_strlen(a) + 1) == 0, message)

/* ========================================================================== */
/* TESTS DE TOKENISATION                                                     */
/* ========================================================================== */

void test_tokenize_basic_word(void)
{
    TEST_START("tokenize_basic_word");
    
    char *input = "echo hello world";
    t_token **tokens = tokenize_input(input);
    
    ASSERT_NOT_NULL(tokens, "Tokens should not be NULL");
    ASSERT_NOT_NULL(*tokens, "First token should not be NULL");
    
    t_token *current = *tokens;
    
    // Premier token: "echo"
    ASSERT_EQUAL(current->type, WORD, "First token should be WORD");
    ASSERT_STR_EQUAL(current->content, "echo", "First token content should be 'echo'");
    
    // Deuxième token: "hello"
    current = current->next;
    ASSERT_NOT_NULL(current, "Second token should exist");
    ASSERT_EQUAL(current->type, WORD, "Second token should be WORD");
    ASSERT_STR_EQUAL(current->content, "hello", "Second token content should be 'hello'");
    
    // Troisième token: "world"
    current = current->next;
    ASSERT_NOT_NULL(current, "Third token should exist");
    ASSERT_EQUAL(current->type, WORD, "Third token should be WORD");
    ASSERT_STR_EQUAL(current->content, "world", "Third token content should be 'world'");
    
    // Pas de quatrième token
    current = current->next;
    ASSERT_NULL(current, "Fourth token should not exist");
    
    free_token_list(tokens);
}

void test_tokenize_quotes(void)
{
    TEST_START("tokenize_quotes");
    
    char *input = "echo 'hello world' \"test string\"";
    t_token **tokens = tokenize_input(input);
    
    ASSERT_NOT_NULL(tokens, "Tokens should not be NULL");
    
    t_token *current = *tokens;
    int token_count = 0;
    
    while (current)
    {
        token_count++;
        if (current->type == SINGLE_QUOTE)
            ASSERT_STR_EQUAL(current->content, "'", "Single quote content");
        else if (current->type == DOUBLE_QUOTE)
            ASSERT_STR_EQUAL(current->content, "\"", "Double quote content");
        current = current->next;
    }
    
    ASSERT_TRUE(token_count > 0, "Should have tokens");
    
    free_token_list(tokens);
}

void test_tokenize_redirections(void)
{
    TEST_START("tokenize_redirections");
    
    char *input = "cat < input.txt > output.txt >> append.txt << EOF";
    t_token **tokens = tokenize_input(input);
    
    ASSERT_NOT_NULL(tokens, "Tokens should not be NULL");
    
    t_token *current = *tokens;
    int redir_count = 0;
    
    while (current)
    {
        if (current->type == REDIR_IN)
        {
            ASSERT_STR_EQUAL(current->content, "<", "Input redirection content");
            redir_count++;
        }
        else if (current->type == REDIR_OUT)
        {
            ASSERT_STR_EQUAL(current->content, ">", "Output redirection content");
            redir_count++;
        }
        else if (current->type == REDIR_APPEND)
        {
            ASSERT_STR_EQUAL(current->content, ">>", "Append redirection content");
            redir_count++;
        }
        else if (current->type == HEREDOC)
        {
            ASSERT_STR_EQUAL(current->content, "<<", "Heredoc content");
            redir_count++;
        }
        current = current->next;
    }
    
    ASSERT_EQUAL(redir_count, 4, "Should have 4 redirection tokens");
    
    free_token_list(tokens);
}

void test_tokenize_operators(void)
{
    TEST_START("tokenize_operators");
    
    char *input = "echo hello | grep hello && echo world || echo failed";
    t_token **tokens = tokenize_input(input);
    
    ASSERT_NOT_NULL(tokens, "Tokens should not be NULL");
    
    t_token *current = *tokens;
    int pipe_count = 0, and_count = 0, or_count = 0;
    
    while (current)
    {
        if (current->type == PIPE)
        {
            ASSERT_STR_EQUAL(current->content, "|", "Pipe content");
            pipe_count++;
        }
        else if (current->type == AND)
        {
            ASSERT_STR_EQUAL(current->content, "&&", "AND content");
            and_count++;
        }
        else if (current->type == OR)
        {
            ASSERT_STR_EQUAL(current->content, "||", "OR content");
            or_count++;
        }
        current = current->next;
    }
    
    ASSERT_EQUAL(pipe_count, 1, "Should have 1 pipe token");
    ASSERT_EQUAL(and_count, 1, "Should have 1 AND token");
    ASSERT_EQUAL(or_count, 1, "Should have 1 OR token");
    
    free_token_list(tokens);
}

void test_tokenize_variables(void)
{
    TEST_START("tokenize_variables");
    
    char *input = "echo $USER $HOME $? $PATH";
    t_token **tokens = tokenize_input(input);
    
    ASSERT_NOT_NULL(tokens, "Tokens should not be NULL");
    
    // Marquer les tokens pour expansion
    mark_tokens_for_expansion(tokens);
    
    t_token *current = *tokens;
    int var_count = 0;
    
    while (current)
    {
        if (current->needs_expansion)
        {
            var_count++;
            if (current->type == ENV_VAR)
                printf("Found env var: %s\n", current->content);
            else if (current->type == EXIT_STATUS)
                printf("Found exit status\n");
        }
        current = current->next;
    }
    
    ASSERT_TRUE(var_count > 0, "Should have variables marked for expansion");
    
    free_token_list(tokens);
}

/* ========================================================================== */
/* TESTS DE VALIDATION SYNTAXIQUE                                            */
/* ========================================================================== */

void test_syntax_validation_quotes(void)
{
    TEST_START("syntax_validation_quotes");
    
    // Test quotes correctes
    char *input1 = "echo 'hello world' \"test string\"";
    t_token **tokens1 = tokenize_input(input1);
    ASSERT_TRUE(quote_syntax_is_valid(tokens1), "Valid quotes should pass");
    free_token_list(tokens1);
    
    // Test quotes non fermées
    char *input2 = "echo 'hello world";
    t_token **tokens2 = tokenize_input(input2);
    ASSERT_FALSE(quote_syntax_is_valid(tokens2), "Unclosed single quote should fail");
    free_token_list(tokens2);
    
    // Test double quotes non fermées
    char *input3 = "echo \"hello world";
    t_token **tokens3 = tokenize_input(input3);
    ASSERT_FALSE(quote_syntax_is_valid(tokens3), "Unclosed double quote should fail");
    free_token_list(tokens3);
}

void test_syntax_validation_parentheses(void)
{
    TEST_START("syntax_validation_parentheses");
    
    // Test parenthèses correctes
    char *input1 = "echo hello && (echo world || echo test)";
    t_token **tokens1 = tokenize_input(input1);
    ASSERT_TRUE(paren_syntax_is_valid(tokens1), "Valid parentheses should pass");
    free_token_list(tokens1);
    
    // Test parenthèses non fermées
    char *input2 = "echo hello && (echo world";
    t_token **tokens2 = tokenize_input(input2);
    ASSERT_FALSE(paren_syntax_is_valid(tokens2), "Unclosed parenthesis should fail");
    free_token_list(tokens2);
    
    // Test parenthèses vides
    char *input3 = "echo hello && ()";
    t_token **tokens3 = tokenize_input(input3);
    ASSERT_FALSE(paren_syntax_is_valid(tokens3), "Empty parentheses should fail");
    free_token_list(tokens3);
}

void test_syntax_validation_redirections(void)
{
    TEST_START("syntax_validation_redirections");
    
    // Test redirections correctes
    char *input1 = "cat < input.txt > output.txt";
    t_token **tokens1 = tokenize_input(input1);
    ASSERT_TRUE(redir_syntax_is_valid(tokens1), "Valid redirections should pass");
    free_token_list(tokens1);
    
    // Test redirection sans cible
    char *input2 = "echo hello >";
    t_token **tokens2 = tokenize_input(input2);
    ASSERT_FALSE(redir_syntax_is_valid(tokens2), "Redirection without target should fail");
    free_token_list(tokens2);
}

void test_syntax_validation_operators(void)
{
    TEST_START("syntax_validation_operators");
    
    // Test opérateurs corrects
    char *input1 = "echo hello && echo world || echo test";
    t_token **tokens1 = tokenize_input(input1);
    ASSERT_TRUE(operator_syntax_is_valid(tokens1), "Valid operators should pass");
    free_token_list(tokens1);
    
    // Test opérateur au début
    char *input2 = "&& echo hello";
    t_token **tokens2 = tokenize_input(input2);
    ASSERT_FALSE(operator_syntax_is_valid(tokens2), "Operator at start should fail");
    free_token_list(tokens2);
    
    // Test opérateur à la fin
    char *input3 = "echo hello &&";
    t_token **tokens3 = tokenize_input(input3);
    ASSERT_FALSE(operator_syntax_is_valid(tokens3), "Operator at end should fail");
    free_token_list(tokens3);
}

/* ========================================================================== */
/* TESTS DE CONSTRUCTION AST                                                 */
/* ========================================================================== */

void test_ast_simple_command(void)
{
    TEST_START("ast_simple_command");
    
    char *input = "echo hello world";
    t_token **tokens = tokenize_input(input);
    t_ast *ast = create_ast(tokens);
    
    ASSERT_NOT_NULL(ast, "AST should not be NULL");
    ASSERT_EQUAL(ast->type, NODE_CMD, "Root should be command node");
    ASSERT_NOT_NULL(ast->cmd.args, "Command args should not be NULL");
    ASSERT_STR_EQUAL(ast->cmd.args[0], "echo", "First arg should be 'echo'");
    ASSERT_STR_EQUAL(ast->cmd.args[1], "hello", "Second arg should be 'hello'");
    ASSERT_STR_EQUAL(ast->cmd.args[2], "world", "Third arg should be 'world'");
    ASSERT_NULL(ast->cmd.args[3], "Fourth arg should be NULL");
    
    free_ast(ast);
    free_token_list(tokens);
}

void test_ast_pipe(void)
{
    TEST_START("ast_pipe");
    
    char *input = "echo hello | grep hello";
    t_token **tokens = tokenize_input(input);
    t_ast *ast = create_ast(tokens);
    
    ASSERT_NOT_NULL(ast, "AST should not be NULL");
    ASSERT_EQUAL(ast->type, NODE_PIPE, "Root should be pipe node");
    ASSERT_NOT_NULL(ast->children, "Pipe should have children");
    ASSERT_NOT_NULL(ast->children[0], "First child should exist");
    ASSERT_NOT_NULL(ast->children[1], "Second child should exist");
    ASSERT_NULL(ast->children[2], "Third child should not exist");
    
    // Premier enfant: echo hello
    ASSERT_EQUAL(ast->children[0]->type, NODE_CMD, "First child should be command");
    ASSERT_STR_EQUAL(ast->children[0]->cmd.args[0], "echo", "First command should be echo");
    
    // Deuxième enfant: grep hello
    ASSERT_EQUAL(ast->children[1]->type, NODE_CMD, "Second child should be command");
    ASSERT_STR_EQUAL(ast->children[1]->cmd.args[0], "grep", "Second command should be grep");
    
    free_ast(ast);
    free_token_list(tokens);
}

void test_ast_logical_operators(void)
{
    TEST_START("ast_logical_operators");
    
    // Test AND
    char *input1 = "echo hello && echo world";
    t_token **tokens1 = tokenize_input(input1);
    t_ast *ast1 = create_ast(tokens1);
    
    ASSERT_NOT_NULL(ast1, "AND AST should not be NULL");
    ASSERT_EQUAL(ast1->type, NODE_AND_IF, "Root should be AND node");
    ASSERT_NOT_NULL(ast1->children, "AND should have children");
    
    free_ast(ast1);
    free_token_list(tokens1);
    
    // Test OR
    char *input2 = "echo hello || echo world";
    t_token **tokens2 = tokenize_input(input2);
    t_ast *ast2 = create_ast(tokens2);
    
    ASSERT_NOT_NULL(ast2, "OR AST should not be NULL");
    ASSERT_EQUAL(ast2->type, NODE_OR_IF, "Root should be OR node");
    ASSERT_NOT_NULL(ast2->children, "OR should have children");
    
    free_ast(ast2);
    free_token_list(tokens2);
}

void test_ast_redirections(void)
{
    TEST_START("ast_redirections");
    
    char *input = "cat < input.txt > output.txt";
    t_token **tokens = tokenize_input(input);
    t_ast *ast = create_ast(tokens);
    
    ASSERT_NOT_NULL(ast, "AST should not be NULL");
    ASSERT_EQUAL(ast->type, NODE_CMD, "Root should be command node");
    ASSERT_NOT_NULL(ast->children, "Command should have redirection children");
    
    // Vérifier les redirections
    int input_redir = 0, output_redir = 0;
    if (ast->children)
    {
        for (int i = 0; ast->children[i]; i++)
        {
            if (ast->children[i]->type == NODE_REDIR_IN)
            {
                input_redir = 1;
                ASSERT_STR_EQUAL(ast->children[i]->file, "input.txt", "Input file should be input.txt");
            }
            else if (ast->children[i]->type == NODE_REDIR_OUT)
            {
                output_redir = 1;
                ASSERT_STR_EQUAL(ast->children[i]->file, "output.txt", "Output file should be output.txt");
            }
        }
    }
    
    ASSERT_TRUE(input_redir, "Should have input redirection");
    ASSERT_TRUE(output_redir, "Should have output redirection");
    
    free_ast(ast);
    free_token_list(tokens);
}

void test_ast_complex_structure(void)
{
    TEST_START("ast_complex_structure");
    
    char *input = "echo hello && (cat file | grep pattern) || echo failed";
    t_token **tokens = tokenize_input(input);
    t_ast *ast = create_ast(tokens);
    
    ASSERT_NOT_NULL(ast, "Complex AST should not be NULL");
    
    // La structure exacte dépend de la priorité des opérateurs
    // Vérifier que l'AST est bien formé
    ASSERT_TRUE(ast->type == NODE_AND_IF || ast->type == NODE_OR_IF, 
                "Root should be logical operator");
    ASSERT_NOT_NULL(ast->children, "Root should have children");
    
    free_ast(ast);
    free_token_list(tokens);
}

/* ========================================================================== */
/* TESTS DE GESTION DES VARIABLES                                            */
/* ========================================================================== */

void test_variable_detection(void)
{
    TEST_START("variable_detection");
    
    char *input = "echo $USER $HOME $? hello$WORLD";
    t_token **tokens = tokenize_input(input);
    
    mark_tokens_for_expansion(tokens);
    
    t_token *current = *tokens;
    int expansion_count = 0;
    
    while (current)
    {
        if (current->needs_expansion)
        {
            expansion_count++;
            printf("Token needing expansion: type=%d, content='%s'\n", 
                   current->type, current->content ? current->content : "NULL");
        }
        current = current->next;
    }
    
    ASSERT_TRUE(expansion_count > 0, "Should have tokens needing expansion");
    
    free_token_list(tokens);
}

void test_variable_in_quotes(void)
{
    TEST_START("variable_in_quotes");
    
    // Variables dans double quotes (doivent être expandues)
    char *input1 = "echo \"Hello $USER\"";
    t_token **tokens1 = tokenize_input(input1);
    mark_tokens_for_expansion(tokens1);
    
    // Variables dans single quotes (ne doivent PAS être expandues)
    char *input2 = "echo 'Hello $USER'";
    t_token **tokens2 = tokenize_input(input2);
    mark_tokens_for_expansion(tokens2);
    
    // Vérifier que le comportement est différent
    // (Les détails dépendent de votre implémentation)
    
    free_token_list(tokens1);
    free_token_list(tokens2);
}

/* ========================================================================== */
/* TESTS DE CAS LIMITES                                                      */
/* ========================================================================== */

void test_edge_cases_empty_input(void)
{
    TEST_START("edge_cases_empty_input");
    
    // Input vide
    char *input1 = "";
    t_token **tokens1 = tokenize_input(input1);
    ASSERT_TRUE(tokens1 == NULL || *tokens1 == NULL, "Empty input should produce no tokens");
    if (tokens1) free_token_list(tokens1);
    
    // Input avec seulement des espaces
    char *input2 = "   \t  \n  ";
    t_token **tokens2 = tokenize_input(input2);
    ASSERT_TRUE(tokens2 == NULL || *tokens2 == NULL, "Whitespace-only input should produce no tokens");
    if (tokens2) free_token_list(tokens2);
}

void test_edge_cases_long_input(void)
{
    TEST_START("edge_cases_long_input");
    
    // Créer une très longue commande
    char long_input[2048] = "echo ";
    for (int i = 0; i < 100; i++)
    {
        char word[20];
        sprintf(word, "word%d ", i);
        strcat(long_input, word);
    }
    
    t_token **tokens = tokenize_input(long_input);
    ASSERT_NOT_NULL(tokens, "Long input should be tokenized");
    
    // Compter les tokens
    t_token *current = *tokens;
    int count = 0;
    while (current)
    {
        count++;
        current = current->next;
    }
    
    ASSERT_TRUE(count > 100, "Should have many tokens for long input");
    
    free_token_list(tokens);
}

void test_edge_cases_special_characters(void)
{
    TEST_START("edge_cases_special_characters");
    
    // Test avec caractères spéciaux non implémentés
    char *input = "echo hello\\world;test";
    t_token **tokens = tokenize_input(input);
    
    // Votre shell ne doit pas crasher même avec des caractères non supportés
    ASSERT_NOT_NULL(tokens, "Should not crash with special characters");
    
    free_token_list(tokens);
}

/* ========================================================================== */
/* FONCTION PRINCIPALE ET RAPPORT                                            */
/* ========================================================================== */

void print_test_summary(void)
{
    printf("\n" BLUE "========================================\n");
    printf("         RÉSUMÉ DES TESTS UNITAIRES\n");
    printf("========================================" RESET "\n");
    printf("Tests exécutés: " CYAN "%d" RESET "\n", g_tests_run);
    printf("Tests réussis:  " GREEN "%d" RESET "\n", g_tests_passed);
    printf("Tests échoués:  " RED "%d" RESET "\n", g_tests_failed);
    
    if (g_tests_failed == 0)
    {
        printf("\n" GREEN "🎉 TOUS LES TESTS UNITAIRES SONT RÉUSSIS !" RESET "\n");
        printf(GREEN "Votre parser fonctionne correctement au niveau unitaire." RESET "\n");
    }
    else
    {
        printf("\n" RED "❌ Certains tests unitaires ont échoué." RESET "\n");
        printf(YELLOW "Veuillez vérifier votre implémentation." RESET "\n");
    }
    
    float success_rate = (float)g_tests_passed / g_tests_run * 100;
    printf("\nTaux de réussite: " CYAN "%.1f%%" RESET "\n", success_rate);
}

int main(void)
{
    printf(PURPLE "============================================\n");
    printf("    TESTS UNITAIRES PARSING MINISHELL\n");
    printf("============================================" RESET "\n\n");
    
    printf(CYAN "Ces tests vérifient individuellement chaque\n");
    printf("composant de votre parser minishell.\n" RESET "\n");
    
    // Exécuter tous les tests
    printf(YELLOW "--- TESTS DE TOKENISATION ---" RESET "\n");
    test_tokenize_basic_word();
    test_tokenize_quotes();
    test_tokenize_redirections();
    test_tokenize_operators();
    test_tokenize_variables();
    
    printf(YELLOW "\n--- TESTS DE VALIDATION SYNTAXIQUE ---" RESET "\n");
    test_syntax_validation_quotes();
    test_syntax_validation_parentheses();
    test_syntax_validation_redirections();
    test_syntax_validation_operators();
    
    printf(YELLOW "\n--- TESTS DE CONSTRUCTION AST ---" RESET "\n");
    test_ast_simple_command();
    test_ast_pipe();
    test_ast_logical_operators();
    test_ast_redirections();
    test_ast_complex_structure();
    
    printf(YELLOW "\n--- TESTS DE GESTION DES VARIABLES ---" RESET "\n");
    test_variable_detection();
    test_variable_in_quotes();
    
    printf(YELLOW "\n--- TESTS DE CAS LIMITES ---" RESET "\n");
    test_edge_cases_empty_input();
    test_edge_cases_long_input();
    test_edge_cases_special_characters();
    
    // Afficher le résumé
    print_test_summary();
    
    return (g_tests_failed == 0) ? 0 : 1;
}