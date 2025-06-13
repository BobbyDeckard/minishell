#!/bin/bash

# Tests détaillés de validation syntaxique pour minishell
# Se concentre sur la détection d'erreurs de parsing

MINISHELL="./minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0

print_test() {
    printf "%-60s" "$1"
}

print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "[${GREEN}PASS${NC}]"
        ((PASSED++))
    else
        echo -e "[${RED}FAIL${NC}]"
        ((FAILED++))
    fi
}

# Test qu'une commande doit échouer (parsing invalide)
test_should_fail() {
    local test_name="$1"
    local command="$2"
    
    print_test "$test_name"
    
    # Redirection vers /dev/null pour éviter l'affichage des erreurs
    timeout 3 bash -c "echo '$command' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    
    # Le test passe si la commande échoue (exit code != 0)
    if [ $exit_code -ne 0 ]; then
        print_result 0
    else
        print_result 1
        echo "    Expected: FAILURE, Got: SUCCESS"
    fi
}

# Test qu'une commande doit réussir (parsing valide)
test_should_pass() {
    local test_name="$1"
    local command="$2"
    
    print_test "$test_name