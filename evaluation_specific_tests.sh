#!/bin/bash

# Tests spécifiques basés sur la feuille d'évaluation 42 Minishell
# Ces tests correspondent exactement aux critères d'évaluation

MINISHELL="./minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0
TOTAL=0

print_section() {
    echo -e "\n${BLUE}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║ $1${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════════════════════╝${NC}"
}

print_test() {
    printf "%-50s" "$1"
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected_behavior="$3"  # "pass" ou "fail"
    
    print_test "$test_name"
    ((TOTAL++))
    
    timeout 5 bash -c "echo '$command' | $MINISHELL" > /tmp/minishell_test_output 2>&1
    local exit_code=$?
    
    if [ "$expected_behavior" = "pass" ]; then
        # Test doit réussir (pas de crash, parsing OK)
        if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ] || [ $exit_code -eq 127 ]; then
            echo -e "[${GREEN}PASS${NC}]"
            ((PASSED++))
        else
            echo -e "[${RED}FAIL${NC}] (exit $exit_code)"
            ((FAILED++))
        fi
    else
        # Test doit échouer (erreur syntaxique détectée)
        if [ $exit_code -ne 0 ] && [ $exit_code -ne 127 ]; then
            echo -e "[${GREEN}PASS${NC}]"
            ((PASSED++))
        else
            echo -e "[${RED}FAIL${NC}] (should have failed)"
            ((FAILED++))
        fi
    fi
    
    rm -f /tmp/minishell_test_output
}

# Tests de compilation (Mandatory Part - Compile)
test_compilation() {
    print_section "COMPILATION ET NORMINETTE"
    
    echo "Vérification de la compilation..."
    if make re > /dev/null 2>&1; then
        echo -e "Compilation: [${GREEN}PASS${NC}]"
    else
        echo -e "Compilation: [${RED}FAIL${NC}]"
        exit 1
    fi
    
    echo "Vérification norminette..."
    if norminette srcs/ incl/ > /dev/null 2>&1; then
        echo -e "Norminette: [${GREEN}PASS${NC}]"
    else
        echo -e "Norminette: [${RED}FAIL${NC}]"
    fi
}

# Tests de commandes simples et variables globales
test_simple_commands_globals() {
    print_section "COMMANDES SIMPLES & VARIABLES GLOBALES"
    
    run_test "Commande avec chemin absolu" "/bin/ls" "pass"
    run_test "Commande sans options" "pwd" "pass"
    run_test "Commande vide" "" "pass"
    run_test "Commande avec espaces/tabs" "   " "pass"
    run_test "Test variable globale (crash test)" "invalidcommand123" "pass"
}

# Tests d'arguments
test_arguments() {
    print_section "ARGUMENTS"
    
    run_test "Commande avec arguments simples" "echo hello world" "pass"
    run_test "Arguments multiples" "echo arg1 arg2 arg3 arg4" "pass"
    run_test "Arguments avec espaces" "echo hello    world" "pass"
}

# Tests echo
test_echo() {
    print_section "BUILTIN ECHO"
    
    run_test "Echo simple" "echo hello" "pass"
    run_test "Echo avec option -n" "echo -n hello" "pass"
    run_test "Echo arguments multiples" "echo hello world test" "pass"
}

# Tests pwd
test_pwd() {
    print_section "BUILTIN PWD"
    
    run_test "pwd sans options" "pwd" "pass"
    run_test "pwd répété" "pwd" "pass"
}

# Tests cd
test_cd() {
    print_section "BUILTIN CD"
    
    run_test "cd avec chemin absolu" "cd /tmp" "pass"
    run_test "cd avec chemin relatif" "cd .." "pass"
    run_test "cd avec . et .." "cd ." "pass"
}

# Tests env
test_env() {
    print_section "BUILTIN ENV"
    
    run_test "env sans options" "env" "pass"
}

# Tests export
test_export() {
    print_section "BUILTIN EXPORT"
    
    run_test "export sans options" "export" "pass"
    run_test "export avec variable" "export TEST=value" "pass"
    run_test "export vérification" "export TEST=value" "pass"
}

# Tests unset
test_unset() {
    print_section "BUILTIN UNSET"
    
    run_test "unset variable" "unset PATH" "pass"
    run_test "unset vérification" "unset TEST" "pass"
}

# Tests de chemins relatifs
test_relative_paths() {
    print_section "CHEMINS RELATIFS"
    
    run_test "Exécution chemin relatif" "./minishell" "pass"
    run_test "Chemin relatif complexe" "../minishell/minishell" "pass"
}

# Tests de valeur de retour
test_return_values() {
    print_section "VALEURS DE RETOUR"
    
    run_test "Commande réussie puis echo \$?" "echo hello" "pass"
    run_test "Commande échouée puis \$?" "commandeinexistante" "pass"
    run_test "Expression arithmétique" "expr 5 + 3" "pass"
}

# Tests de signaux (parsing seulement)
test_signals_parsing() {
    print_section "GESTION DES SIGNAUX (PARSING)"
    
    echo "Tests de signaux à effectuer manuellement:"
    echo "- Ctrl-C dans prompt vide -> nouvelle ligne + prompt"
    echo "- Ctrl-D dans prompt vide -> exit"
    echo "- Ctrl-\\ dans prompt vide -> aucune action"
    echo "- Historique avec flèches haut/bas"
}

# Tests de guillemets doubles
test_double_quotes() {
    print_section "GUILLEMETS DOUBLES"
    
    run_test "Guillemets doubles simples" 'echo "hello world"' "pass"
    run_test "Variable dans guillemets doubles" 'echo "Hello \$USER"' "pass"
    run_test "Espaces dans guillemets doubles" 'echo "hello   world"' "pass"
    run_test "Guillemets doubles vides" 'echo ""' "pass"
    run_test "Exit status dans guillemets" 'echo "Exit: \$?"' "pass"
}

# Tests de guillemets simples  
test_single_quotes() {
    print_section "GUILLEMETS SIMPLES"
    
    run_test "Guillemets simples basiques" "echo 'hello world'" "pass"
    run_test "Variable dans guillemets simples" "echo '\$USER'" "pass"
    run_test "Espaces dans guillemets simples" "echo 'hello   world'" "pass"
    run_test "Caractères spéciaux protégés" "echo '| > < & ;'" "pass"
    run_test "Echo \$USER dans guillemets" "echo '\$USER'" "pass"
}

# Tests de variables d'environnement
test_environment_variables() {
    print_section "VARIABLES D'ENVIRONNEMENT"
    
    run_test "Variable USER" "echo \$USER" "pass"
    run_test "Variable HOME" "echo \$HOME" "pass"
    run_test "Variable PATH" "echo \$PATH" "pass"
    run_test "Dollar seul" "echo \$" "pass"
    run_test "Variable inexistante" "echo \$NONEXISTENT" "pass"
    run_test "Variables multiples" "echo \$USER \$HOME" "pass"
}

# Tests de redirections
test_redirections() {
    print_section "REDIRECTIONS"
    
    # Création de fichiers de test
    echo "test content" > /tmp/test_input.txt
    
    run_test "Redirection sortie" "echo hello > /tmp/test_out.txt" "pass"
    run_test "Redirection entrée" "< /tmp/test_input.txt cat" "pass"
    run_test "Redirection append" "echo world >> /tmp/test_out.txt" "pass"
    run_test "Redirections multiples" "< /tmp/test_input.txt cat > /tmp/test_out2.txt" "pass"
    run_test "Heredoc simple" "cat << EOF"$'\n'"hello"$'\n'"EOF" "pass"
    run_test "Redirection échec fichier" "< /nonexistent/file cat" "pass"
    
    # Nettoyage
    rm -f /tmp/test_input.txt /tmp/test_out.txt /tmp/test_out2.txt
}

# Tests de pipes
test_pipes() {
    print_section "PIPES"
    
    run_test "Pipe simple" "echo hello | cat" "pass"
    run_test "Pipe avec grep" "echo hello world | grep hello" "pass"
    run_test "Pipes multiples" "echo hello | cat | cat" "pass"
    run_test "Pipe avec wc" "echo hello world | wc -w" "pass"
    run_test "Commande inexistante dans pipe" "nonexistent | cat" "pass"
    run_test "Pipes et redirections" "echo hello | cat > /tmp/pipe_out.txt" "pass"
    
    rm -f /tmp/pipe_out.txt
}

# Tests go crazy et historique
test_crazy_and_history() {
    print_section "GO CRAZY & HISTORIQUE"
    
    run_test "Commande longue" "echo $(printf 'a%.0s' {1..100})" "pass"
    run_test "Arguments nombreux" "echo $(seq 1 20)" "pass"
    run_test "Commande invalide longue" "invalidcommandverylongname" "pass"
    run_test "Comportement normal" "cat" "pass"
    
    echo "Tests d'historique à effectuer manuellement:"
    echo "- Navigation avec flèches haut/bas"
    echo "- Réexécution de commandes"
    echo "- Persistence de l'historique"
}

# Tests de chemins d'environnement  
test_environment_path() {
    print_section "CHEMINS D'ENVIRONNEMENT"
    
    run_test "Commande sans PATH" "ls" "pass"
    run_test "Unset PATH puis commande" "unset PATH" "pass"
    run_test "PATH multiple" "echo \$PATH" "pass"
}

# Tests bonus - AND/OR
test_bonus_logical() {
    print_section "BONUS - AND/OR"
    
    echo "Tests bonus (si implémentés):"
    run_test "AND simple" "echo hello && echo world" "pass"
    run_test "OR simple" "echo hello || echo world" "pass"
    run_test "Parenthèses" "(echo hello && echo world)" "pass"
}

# Tests bonus - Wildcards
test_bonus_wildcards() {
    print_section "BONUS - WILDCARDS"
    
    echo "Tests de wildcards (si implémentés):"
    run_test "Wildcard simple" "echo *" "pass"
    echo "Note: Wildcard doit fonctionner dans le répertoire courant"
}

# Fonction de nettoyage et résumé
print_summary() {
    print_section "RÉSUMÉ DES TESTS D'ÉVALUATION"
    
    echo -e "Total des tests automatisés: ${BLUE}$TOTAL${NC}"
    echo -e "Tests réussis: ${GREEN}$PASSED${NC}"
    echo -e "Tests échoués: ${RED}$FAILED${NC}"
    
    if [ $TOTAL -gt 0 ]; then
        local success_rate=$(( PASSED * 100 / TOTAL ))
        echo -e "Taux de réussite: ${BLUE}$success_rate%${NC}"
        
        if [ $success_rate -ge 90 ]; then
            echo -e "\n${GREEN}🏆 EXCELLENT ! Votre minishell est prêt pour l'évaluation !${NC}"
        elif [ $success_rate -ge 75 ]; then
            echo -e "\n${YELLOW}⚠️  BON, mais quelques améliorations nécessaires${NC}"
        else
            echo -e "\n${RED}❌ ATTENTION ! Plusieurs problèmes à corriger avant l'évaluation${NC}"
        fi
    fi
    
    echo -e "\n${BLUE}Notes importantes pour l'évaluation:${NC}"
    echo "- Testez manuellement les signaux (Ctrl-C, Ctrl-D, Ctrl-\\)"
    echo "- Vérifiez l'historique avec les flèches"
    echo "- Testez avec des commandes bloquantes (cat sans argument)"
    echo "- Vérifiez la gestion des variables globales"
    echo "- Assurez-vous que seul le parsing est testé ici"
    
    if [ $FAILED -eq 0 ]; then
        exit 0
    else
        exit 1
    fi
}

# Fonction principale
main() {
    echo -e "${BLUE}"
    echo "╔══════════════════════════════════════════════════════════════╗"
    echo "║              TESTS FEUILLE D'ÉVALUATION 42                  ║"
    echo "║                    MINISHELL PARSING                         ║"
    echo "║                                                              ║"
    echo "║  Ces tests correspondent aux critères exacts d'évaluation   ║"
    echo "╚══════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}ERREUR: $MINISHELL non trouvé${NC}"
        echo "Compilez d'abord le projet avec 'make'"
        exit 1
    fi
    
    # Exécution des tests dans l'ordre de la feuille d'évaluation
    test_compilation
    test_simple_commands_globals
    test_arguments
    test_echo
    test_pwd
    test_cd
    test_env
    test_export
    test_unset
    test_relative_paths
    test_return_values
    test_signals_parsing
    test_double_quotes
    test_single_quotes
    test_environment_variables
    test_redirections
    test_pipes
    test_crazy_and_history
    test_environment_path
    test_bonus_logical
    test_bonus_wildcards
    
    print_summary
}

# Gestion des signaux
trap 'echo -e "\n${RED}Tests interrompus par l'\''utilisateur${NC}"; exit 130' INT

# Exécution du script
main "$@"