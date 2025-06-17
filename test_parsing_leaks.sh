#!/bin/bash

# ==================================================================================
# ULTRA SIMPLE PARSING LEAK TESTER
# ==================================================================================
# Version ultra-simple qui utilise votre minishell tel quel
# ==================================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

print_header() {
    echo -e "${CYAN}════════════════════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}                         ULTRA SIMPLE PARSING LEAK TESTER${NC}"
    echo -e "${CYAN}════════════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_info() {
    echo -e "${CYAN}ℹ${NC} $1"
}

print_test() {
    echo -e "${BLUE}[TEST]${NC} $1"
}

# Test une seule commande
test_single_command() {
    local test_name="$1"
    local command="$2"
    
    print_test "$test_name: \"$command\""
    
    # Créer fichier temporaire avec la commande + exit
    local temp_file="/tmp/minishell_test_$$"
    echo "$command" > "$temp_file"
    echo "exit" >> "$temp_file"
    
    # Log valgrind
    local log_file="leak_${test_name// /_}.log"
    
    # Exécuter avec valgrind (timeout 3 secondes)
    timeout 3 valgrind \
        --leak-check=full \
        --show-leak-kinds=definite \
        --track-origins=yes \
        --error-exitcode=42 \
        --log-file="$log_file" \
        ./minishell < "$temp_file" >/dev/null 2>&1
    
    local exit_code=$?
    rm -f "$temp_file"
    
    # Analyser résultats
    if [ -f "$log_file" ]; then
        local definitely_lost=$(grep "definitely lost:" "$log_file" | grep -o '[0-9,]* bytes' | head -1 | tr -d ',')
        local error_count=$(grep "ERROR SUMMARY:" "$log_file" | grep -o '[0-9]*' | head -1)
        
        if [ -z "$definitely_lost" ] || [ "$definitely_lost" = "0" ]; then
            if [ -z "$error_count" ] || [ "$error_count" = "0" ]; then
                print_success "Pas de fuites détectées"
                rm -f "$log_file"
                return 0
            fi
        fi
        
        print_error "Fuites détectées: ${definitely_lost:-0} bytes, ${error_count:-0} erreurs"
        echo "         Log sauvé: $log_file"
        return 1
    else
        print_error "Impossible de créer le log"
        return 1
    fi
}

# Vérifications préliminaires
check_setup() {
    echo -e "${BLUE}[SETUP]${NC} Vérification de l'environnement"
    
    # Valgrind
    if ! command -v valgrind &> /dev/null; then
        print_error "Valgrind non installé"
        exit 1
    fi
    print_success "Valgrind disponible"
    
    # Minishell
    if [ ! -f "./minishell" ]; then
        print_info "Compilation de minishell..."
        if make >/dev/null 2>&1; then
            print_success "Minishell compilé"
        else
            print_error "Impossible de compiler minishell"
            print_info "Essayez: make clean && make"
            exit 1
        fi
    else
        print_success "Minishell trouvé"
    fi
    
    echo ""
}

# Tests de base
run_basic_tests() {
    echo -e "${YELLOW}═══ TESTS DE PARSING DE BASE ═══${NC}"
    echo ""
    
    local total=0
    local passed=0
    
    # Tests simples
    test_single_command "Echo simple" "echo hello" && ((passed++))
    ((total++))
    
    test_single_command "Arguments multiples" "echo hello world test" && ((passed++))
    ((total++))
    
    test_single_command "Quotes simples" "echo 'hello world'" && ((passed++))
    ((total++))
    
    test_single_command "Quotes doubles" "echo \"hello world\"" && ((passed++))
    ((total++))
    
    test_single_command "Variable HOME" "echo \$HOME" && ((passed++))
    ((total++))
    
    test_single_command "Exit status" "echo \$?" && ((passed++))
    ((total++))
    
    echo ""
    echo -e "${CYAN}Résultat parsing de base: $passed/$total tests réussis${NC}"
    echo ""
    
    return $((total - passed))
}

# Tests d'opérateurs
run_operator_tests() {
    echo -e "${YELLOW}═══ TESTS D'OPÉRATEURS ═══${NC}"
    echo ""
    
    local total=0
    local passed=0
    
    test_single_command "Pipe simple" "echo hello | cat" && ((passed++))
    ((total++))
    
    test_single_command "Redirection out" "echo hello > /tmp/test_out" && ((passed++))
    ((total++))
    
    test_single_command "Redirection in" "cat < /dev/null" && ((passed++))
    ((total++))
    
    test_single_command "AND operator" "echo hello && echo world" && ((passed++))
    ((total++))
    
    test_single_command "OR operator" "false || echo backup" && ((passed++))
    ((total++))
    
    echo ""
    echo -e "${CYAN}Résultat opérateurs: $passed/$total tests réussis${NC}"
    echo ""
    
    return $((total - passed))
}

# Tests complexes
run_complex_tests() {
    echo -e "${YELLOW}═══ TESTS COMPLEXES ═══${NC}"
    echo ""
    
    local total=0
    local passed=0
    
    test_single_command "Pipe multiple" "echo hello | cat | wc -l" && ((passed++))
    ((total++))
    
    test_single_command "Variables multiples" "echo \$HOME \$USER" && ((passed++))
    ((total++))
    
    test_single_command "Parenthèses" "(echo hello && echo world)" && ((passed++))
    ((total++))
    
    test_single_command "Commande complexe" "echo test | cat > /tmp/complex_test" && ((passed++))
    ((total++))
    
    echo ""
    echo -e "${CYAN}Résultat complexes: $passed/$total tests réussis${NC}"
    echo ""
    
    return $((total - passed))
}

# Tests d'erreurs
run_error_tests() {
    echo -e "${YELLOW}═══ TESTS D'ERREURS SYNTAXIQUES ═══${NC}"
    echo ""
    
    local total=0
    local passed=0
    
    # Ces tests devraient échouer syntaxiquement mais sans fuites
    test_single_command "Quotes non fermées" "echo 'hello" && ((passed++))
    ((total++))
    
    test_single_command "Pipe orphelin" "echo hello |" && ((passed++))
    ((total++))
    
    test_single_command "Redirection vide" "echo >" && ((passed++))
    ((total++))
    
    echo ""
    echo -e "${CYAN}Résultat erreurs: $passed/$total tests réussis${NC}"
    echo ""
    
    return $((total - passed))
}

# Résumé final
print_summary() {
    local total_failed=$1
    
    echo ""
    echo -e "${CYAN}════════════════════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}                                    RÉSUMÉ FINAL${NC}"
    echo -e "${CYAN}════════════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
    
    if [ "$total_failed" -eq 0 ]; then
        echo -e "${GREEN}🎉 EXCELLENT ! Aucune fuite de parsing détectée !${NC}"
        echo ""
        echo -e "${GREEN}Votre système de parsing semble propre au niveau mémoire.${NC}"
    elif [ "$total_failed" -le 3 ]; then
        echo -e "${YELLOW}⚠️  Quelques fuites détectées ($total_failed tests échoués)${NC}"
        echo ""
        echo -e "${YELLOW}Des améliorations mineures sont nécessaires.${NC}"
    else
        echo -e "${RED}❌ Plusieurs fuites détectées ($total_failed tests échoués)${NC}"
        echo ""
        echo -e "${RED}Le parsing nécessite une révision importante.${NC}"
    fi
    
    echo ""
    echo -e "${CYAN}📄 Logs détaillés sauvés dans:${NC} leak_*.log"
    echo -e "${CYAN}🔍 Pour analyser:${NC} grep 'definitely lost' leak_*.log"
    echo -e "${CYAN}🧹 Pour nettoyer:${NC} rm -f leak_*.log"
    echo ""
    
    if [ "$total_failed" -gt 0 ]; then
        echo -e "${YELLOW}💡 Conseils pour corriger les fuites:${NC}"
        echo "   1. Vérifiez free_token_list() et free_ast()"
        echo "   2. Libérez les tokens après create_ast()"
        echo "   3. Gérez les cas d'erreur de parsing"
        echo "   4. Initialisez la mémoire avant ft_strlcat()"
        echo ""
    fi
}

# Fonction principale
main() {
    print_header
    check_setup
    
    local failed_basic=0
    local failed_operators=0
    local failed_complex=0
    local failed_errors=0
    
    run_basic_tests
    failed_basic=$?
    
    run_operator_tests
    failed_operators=$?
    
    run_complex_tests
    failed_complex=$?
    
    run_error_tests
    failed_errors=$?
    
    local total_failed=$((failed_basic + failed_operators + failed_complex + failed_errors))
    
    print_summary "$total_failed"
    
    # Nettoyer les fichiers temporaires
    rm -f /tmp/test_out /tmp/complex_test
    
    # Code de sortie
    if [ "$total_failed" -eq 0 ]; then
        exit 0
    else
        exit 1
    fi
}

# Exécuter
main "$@"