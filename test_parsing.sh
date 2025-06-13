#!/bin/bash

# Script de test automatisé pour le parsing du minishell
# Usage: ./test_parsing.sh

# Configuration
MINISHELL="./minishell"
TEST_DIR="parsing_tests"
RESULTS_FILE="parsing_test_results.txt"
TIMEOUT=5
PASSED=0
FAILED=0
TOTAL=0

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Fonction d'affichage
print_header() {
    echo -e "${BLUE}================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}================================${NC}"
}

print_test() {
    printf "%-50s" "$1"
}

print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "[${GREEN}PASS${NC}]"
        ((PASSED++))
    else
        echo -e "[${RED}FAIL${NC}]"
        ((FAILED++))
    fi
    ((TOTAL++))
}

# Préparation
setup_tests() {
    print_header "SETUP - Préparation des tests"
    
    # Compilation du minishell
    echo "Compilation du minishell..."
    make re > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}ERREUR: Compilation échouée${NC}"
        exit 1
    fi
    
    # Création du répertoire de test
    mkdir -p "$TEST_DIR"
    cd "$TEST_DIR"
    
    # Création de fichiers de test
    echo "hello world" > test_input.txt
    echo -e "line1\nline2\nline3" > multiline.txt
    touch empty_file.txt
    
    # Initialisation du fichier de résultats
    echo "=== Résultats des Tests de Parsing Minishell ===" > "../$RESULTS_FILE"
    echo "Date: $(date)" >> "../$RESULTS_FILE"
    echo "" >> "../$RESULTS_FILE"
    
    cd ..
    echo "Setup terminé."
}

# Test de compilation et norminette
test_compilation() {
    print_header "TESTS DE COMPILATION"
    
    print_test "Compilation avec flags norminette"
    make re CFLAGS="-Wall -Wextra -Werror" > /dev/null 2>&1
    print_result $?
    
    print_test "Vérification norminette (parsing)"
    norminette srcs/parsing/ incl/parsing.h > /dev/null 2>&1
    print_result $?
}

# Fonction pour exécuter un test simple
run_simple_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"
    
    print_test "$test_name"
    
    # Exécution avec timeout
    timeout $TIMEOUT bash -c "echo '$command' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    
    if [ -z "$expected_exit" ]; then
        # Test de non-crash (exit code 0 ou 1 acceptable)
        if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ] || [ $exit_code -eq 127 ]; then
            print_result 0
        else
            print_result 1
        fi
    else
        # Test avec exit code attendu
        if [ $exit_code -eq $expected_exit ]; then
            print_result 0
        else
            print_result 1
        fi
    fi
}

# Tests de commandes simples
test_simple_commands() {
    print_header "TESTS DE COMMANDES SIMPLES"
    
    run_simple_test "Commande pwd" "pwd"
    run_simple_test "Echo simple" "echo hello"
    run_simple_test "Echo avec arguments" "echo hello world test"
    run_simple_test "Chemin absolu" "/bin/echo hello"
    run_simple_test "Commande inexistante" "commandeinexistante" 127
    run_simple_test "Arguments multiples" "echo arg1 arg2 arg3 arg4 arg5"
    run_simple_test "Espaces multiples" "echo    hello    world   "
    run_simple_test "Commande vide" ""
    run_simple_test "Seulement espaces" "   "
}

# Tests de variables d'environnement
test_environment_vars() {
    print_header "TESTS DE VARIABLES D'ENVIRONNEMENT"
    
    run_simple_test "Variable USER" "echo \$USER"
    run_simple_test "Variable inexistante" "echo \$VARIABLE_INEXISTANTE"
    run_simple_test "Exit status" "echo \$?"
    run_simple_test "Variable avec texte" "echo Hello\$USER"
    run_simple_test "Variables multiples" "echo \$USER \$HOME"
    run_simple_test "Dollar seul" "echo \$"
    run_simple_test "Variable avec underscore" "echo \$MY_VAR"
}

# Tests de guillemets simples
test_single_quotes() {
    print_header "TESTS DE GUILLEMETS SIMPLES"
    
    run_simple_test "Guillemets simples de base" "echo 'hello world'"
    run_simple_test "Variable dans guillemets simples" "echo '\$USER'"
    run_simple_test "Guillemets simples vides" "echo ''"
    run_simple_test "Espaces dans guillemets simples" "echo 'hello   world'"
    run_simple_test "Caractères spéciaux" "echo '| > < & ;'"
}

# Tests de guillemets doubles
test_double_quotes() {
    print_header "TESTS DE GUILLEMETS DOUBLES"
    
    run_simple_test "Guillemets doubles de base" 'echo "hello world"'
    run_simple_test "Variable dans guillemets doubles" 'echo "Hello \$USER"'
    run_simple_test "Guillemets doubles vides" 'echo ""'
    run_simple_test "Espaces dans guillemets doubles" 'echo "hello   world"'
    run_simple_test "Exit status dans guillemets doubles" 'echo "Exit: \$?"'
}

# Tests de redirections
test_redirections() {
    print_header "TESTS DE REDIRECTIONS"
    
    cd "$TEST_DIR"
    
    run_simple_test "Redirection sortie simple" "echo hello > output.txt"
    run_simple_test "Redirection entrée simple" "< test_input.txt cat"
    run_simple_test "Redirection append" "echo world >> output.txt"
    run_simple_test "Fichier inexistant en entrée" "< fichier_inexistant cat" 1
    
    # Nettoyage
    rm -f output.txt
    cd ..
}

# Tests de pipes
test_pipes() {
    print_header "TESTS DE PIPES"
    
    run_simple_test "Pipe simple" "echo hello | cat"
    run_simple_test "Pipe avec grep" "echo hello world | grep hello"
    run_simple_test "Pipes multiples" "echo hello | cat | cat"
    run_simple_test "Pipe avec wc" "echo hello world | wc -w"
    run_simple_test "Commande inexistante dans pipe" "commandeinexistante | cat" 127
}

# Tests de builtins
test_builtins() {
    print_header "TESTS DE BUILTINS (PARSING)"
    
    run_simple_test "Echo simple" "echo hello"
    run_simple_test "Echo avec -n" "echo -n hello"
    run_simple_test "Pwd" "pwd"
    run_simple_test "Env" "env"
    run_simple_test "Export simple" "export TEST=value"
}

# Tests de validation syntaxique
test_syntax_validation() {
    print_header "TESTS DE VALIDATION SYNTAXIQUE"
    
    run_simple_test "Pipe en début (erreur)" "| cat" 1
    run_simple_test "Pipe en fin (erreur)" "cat |" 1
    run_simple_test "Redirection sans fichier" "cat >" 1
    run_simple_test "Guillemets non fermés" 'echo "hello' 1
}

# Tests de robustesse
test_robustness() {
    print_header "TESTS DE ROBUSTESSE"
    
    # Test de commande longue
    local long_cmd="echo $(printf 'a%.0s' {1..100})"
    run_simple_test "Commande longue" "$long_cmd"
    
    # Test de stress rapide
    print_test "Test de stress (10 commandes)"
    local stress_pass=0
    for i in {1..10}; do
        timeout 2 bash -c "echo 'echo test$i' | $MINISHELL" > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            ((stress_pass++))
        fi
    done
    
    if [ $stress_pass -ge 8 ]; then
        print_result 0
    else
        print_result 1
    fi
}

# Test de memory leaks
test_memory() {
    print_header "TESTS DE MÉMOIRE"
    
    if command -v valgrind &> /dev/null; then
        print_test "Test Valgrind (memory leaks)"
        timeout 10 valgrind --leak-check=full --error-exitcode=1 \
            bash -c "echo 'echo hello' | $MINISHELL" > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            print_result 0
        else
            print_result 1
        fi
    else
        print_test "Valgrind non disponible"
        echo -e "[${YELLOW}SKIP${NC}]"
    fi
}

# Nettoyage
cleanup() {
    print_header "NETTOYAGE"
    
    cd "$TEST_DIR"
    rm -f output.txt test_input.txt multiline.txt empty_file.txt
    cd ..
    rmdir "$TEST_DIR" 2>/dev/null
    
    echo "Nettoyage terminé."
}

# Affichage des résultats finaux
print_summary() {
    print_header "RÉSUMÉ DES TESTS"
    
    echo -e "Total des tests: ${BLUE}$TOTAL${NC}"
    echo -e "Tests réussis: ${GREEN}$PASSED${NC}"
    echo -e "Tests échoués: ${RED}$FAILED${NC}"
    
    local success_rate=$(( PASSED * 100 / TOTAL ))
    echo -e "Taux de réussite: ${BLUE}$success_rate%${NC}"
    
    # Sauvegarde des résultats
    {
        echo ""
        echo "=== RÉSUMÉ ==="
        echo "Total: $TOTAL"
        echo "Réussis: $PASSED"
        echo "Échoués: $FAILED"
        echo "Taux de réussite: $success_rate%"
    } >> "$RESULTS_FILE"
    
    if [ $FAILED -eq 0 ]; then
        echo -e "\n${GREEN}🎉 TOUS LES TESTS SONT PASSÉS !${NC}"
    else
        echo -e "\n${RED}❌ $FAILED test(s) ont échoué${NC}"
        echo -e "Voir $RESULTS_FILE pour plus de détails"
    fi
}

# Fonction principale
main() {
    echo -e "${BLUE}"
    echo "╔══════════════════════════════════════════════════════════════╗"
    echo "║               TESTS DE PARSING - MINISHELL                  ║"
    echo "║                     Version 2024                            ║"
    echo "╚══════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    
    # Vérification de l'existence du minishell
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}ERREUR: $MINISHELL non trouvé${NC}"
        echo "Compilez d'abord le projet avec 'make'"
        exit 1
    fi
    
    # Exécution des tests
    setup_tests
    test_compilation
    test_simple_commands
    test_environment_vars
    test_single_quotes
    test_double_quotes
    test_redirections
    test_pipes
    test_builtins
    test_syntax_validation
    test_robustness
    test_memory
    cleanup
    print_summary
    
    # Code de sortie
    if [ $FAILED -eq 0 ]; then
        exit 0
    else
        exit 1
    fi
}

# Gestion des signaux
trap 'echo -e "\n${RED}Tests interrompus${NC}"; cleanup; exit 130' INT

# Exécution
main "$@"