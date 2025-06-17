#!/bin/bash

# ==================================================================================
# MINISHELL LEAK TESTING SCRIPT
# ==================================================================================
# Ce script teste votre minishell pour détecter les fuites de mémoire
# avec valgrind et analyse les résultats de manière détaillée.
#
# Usage: ./test_leaks.sh [options]
# Options:
#   -v, --verbose    Mode verbeux avec plus de détails
#   -q, --quick      Tests rapides seulement
#   -f, --full       Tests complets avec tous les cas complexes
#   -s, --summary    Affiche seulement le résumé final
# ==================================================================================

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Variables globales
MINISHELL_PATH="./minishell"
VALGRIND_LOG_DIR="valgrind_logs"
VERBOSE=false
QUICK=false
FULL=false
SUMMARY_ONLY=false
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# ==================================================================================
# FONCTIONS UTILITAIRES
# ==================================================================================

print_header() {
    echo -e "${CYAN}================================================================================${NC}"
    echo -e "${CYAN}                         MINISHELL LEAK TESTING SCRIPT${NC}"
    echo -e "${CYAN}================================================================================${NC}"
    echo ""
}

print_test_header() {
    local test_name="$1"
    echo -e "${BLUE}[TEST]${NC} $test_name"
    echo -e "${BLUE}────────────────────────────────────────────────────────────────────────────${NC}"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_info() {
    echo -e "${CYAN}ℹ${NC} $1"
}

# ==================================================================================
# VÉRIFICATIONS PRÉLIMINAIRES
# ==================================================================================

check_prerequisites() {
    print_test_header "Vérification des prérequis"
    
    # Vérifier si valgrind est installé
    if ! command -v valgrind &> /dev/null; then
        print_error "Valgrind n'est pas installé. Installation requise :"
        echo "  Ubuntu/Debian: sudo apt-get install valgrind"
        echo "  macOS: brew install valgrind (peut ne pas être disponible sur M1/M2)"
        exit 1
    fi
    print_success "Valgrind est disponible"
    
    # Vérifier si minishell existe et est exécutable
    if [ ! -f "$MINISHELL_PATH" ]; then
        print_error "Minishell non trouvé à $MINISHELL_PATH"
        print_info "Tentative de compilation..."
        if make > /dev/null 2>&1; then
            print_success "Compilation réussie"
        else
            print_error "Échec de la compilation"
            exit 1
        fi
    fi
    
    if [ ! -x "$MINISHELL_PATH" ]; then
        print_error "Minishell n'est pas exécutable"
        exit 1
    fi
    print_success "Minishell trouvé et exécutable"
    
    # Créer le dossier de logs
    mkdir -p "$VALGRIND_LOG_DIR"
    print_success "Dossier de logs créé : $VALGRIND_LOG_DIR"
    
    echo ""
}

# ==================================================================================
# FONCTIONS DE TEST
# ==================================================================================

run_valgrind_test() {
    local test_name="$1"
    local commands="$2"
    local timeout_duration="${3:-10}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    local log_file="$VALGRIND_LOG_DIR/${test_name// /_}.log"
    local temp_script="/tmp/minishell_test_$$"
    
    # Créer un script temporaire avec les commandes
    echo "$commands" > "$temp_script"
    echo "exit" >> "$temp_script"
    
    if [ "$VERBOSE" = true ] || [ "$SUMMARY_ONLY" = false ]; then
        echo -e "  ${YELLOW}→${NC} Test: $test_name"
    fi
    
    # Exécuter valgrind avec timeout
    timeout "$timeout_duration" valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --track-fds=yes \
        --error-exitcode=42 \
        --log-file="$log_file" \
        "$MINISHELL_PATH" < "$temp_script" > /dev/null 2>&1
    
    local exit_code=$?
    rm -f "$temp_script"
    
    # Analyser les résultats
    analyze_valgrind_output "$test_name" "$log_file" "$exit_code"
}

analyze_valgrind_output() {
    local test_name="$1"
    local log_file="$2"
    local exit_code="$3"
    
    if [ ! -f "$log_file" ]; then
        print_error "Log file non créé pour $test_name"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return
    fi
    
    # Extraire les informations importantes
    local definitely_lost=$(grep "definitely lost:" "$log_file" | grep -o '[0-9,]* bytes' | head -1)
    local indirectly_lost=$(grep "indirectly lost:" "$log_file" | grep -o '[0-9,]* bytes' | head -1)
    local possibly_lost=$(grep "possibly lost:" "$log_file" | grep -o '[0-9,]* bytes' | head -1)
    local still_reachable=$(grep "still reachable:" "$log_file" | grep -o '[0-9,]* bytes' | head -1)
    local error_count=$(grep "ERROR SUMMARY:" "$log_file" | grep -o '[0-9]*' | head -1)
    local fd_leaks=$(grep "Open file descriptor" "$log_file" | wc -l)
    
    # Déterminer si le test a réussi
    local has_leaks=false
    local has_errors=false
    
    if [ "$error_count" != "0" ] && [ -n "$error_count" ]; then
        has_errors=true
    fi
    
    if echo "$definitely_lost" | grep -q -v "0 bytes"; then
        has_leaks=true
    fi
    
    if [ "$fd_leaks" -gt 0 ]; then
        has_leaks=true
    fi
    
    # Afficher les résultats
    if [ "$has_leaks" = false ] && [ "$has_errors" = false ]; then
        if [ "$SUMMARY_ONLY" = false ]; then
            print_success "$test_name - Aucune fuite détectée"
        fi
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        if [ "$SUMMARY_ONLY" = false ]; then
            print_error "$test_name - Fuites détectées"
            if [ "$VERBOSE" = true ]; then
                echo "    Definitely lost: ${definitely_lost:-0 bytes}"
                echo "    Indirectly lost: ${indirectly_lost:-0 bytes}"
                echo "    Possibly lost: ${possibly_lost:-0 bytes}"
                echo "    Still reachable: ${still_reachable:-0 bytes}"
                echo "    Errors: ${error_count:-0}"
                echo "    FD leaks: $fd_leaks"
                echo "    Log: $log_file"
            fi
        fi
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# ==================================================================================
# ENSEMBLES DE TESTS
# ==================================================================================

test_basic_commands() {
    print_test_header "Tests des commandes de base"
    
    run_valgrind_test "Echo simple" "echo hello"
    run_valgrind_test "Echo avec -n" "echo -n hello"
    run_valgrind_test "PWD" "pwd"
    run_valgrind_test "ENV" "env"
    run_valgrind_test "Commande externe simple" "ls"
    run_valgrind_test "Commande avec arguments" "ls -la"
    
    echo ""
}

test_redirections() {
    print_test_header "Tests des redirections"
    
    # Créer des fichiers de test
    echo "test content" > /tmp/test_input.txt
    
    run_valgrind_test "Redirection sortie" "echo hello > /tmp/test_output.txt"
    run_valgrind_test "Redirection entrée" "cat < /tmp/test_input.txt"
    run_valgrind_test "Redirection append" "echo world >> /tmp/test_output.txt"
    run_valgrind_test "Heredoc simple" "cat << EOF
line1
line2
EOF"
    
    # Nettoyer
    rm -f /tmp/test_input.txt /tmp/test_output.txt
    
    echo ""
}

test_pipes() {
    print_test_header "Tests des pipes"
    
    run_valgrind_test "Pipe simple" "echo hello | cat"
    run_valgrind_test "Pipe double" "echo hello | cat | wc -l"
    run_valgrind_test "Pipe triple" "echo hello world | tr ' ' '\n' | wc -l"
    run_valgrind_test "Pipe avec builtin" "echo hello | cat | wc"
    
    echo ""
}

test_environment_variables() {
    print_test_header "Tests des variables d'environnement"
    
    run_valgrind_test "Variable HOME" "echo \$HOME"
    run_valgrind_test "Variable PATH" "echo \$PATH"
    run_valgrind_test "Exit status" "echo \$?"
    run_valgrind_test "Export variable" "export TEST_VAR=hello"
    run_valgrind_test "Unset variable" "unset TEST_VAR"
    
    echo ""
}

test_builtins() {
    print_test_header "Tests des builtins"
    
    run_valgrind_test "CD home" "cd ~"
    run_valgrind_test "CD relative" "cd .."
    run_valgrind_test "Export sans argument" "export"
    run_valgrind_test "Unset inexistant" "unset NONEXISTENT_VAR"
    
    echo ""
}

test_complex_scenarios() {
    print_test_header "Tests de scénarios complexes"
    
    run_valgrind_test "Commandes multiples" "echo hello; echo world; pwd"
    run_valgrind_test "Pipe avec redirection" "echo hello | cat > /tmp/complex_test.txt"
    run_valgrind_test "Variables et pipes" "echo \$HOME | cat"
    run_valgrind_test "Heredoc avec pipe" "cat << EOF | wc -l
line1
line2
line3
EOF"
    
    # Nettoyer
    rm -f /tmp/complex_test.txt
    
    echo ""
}

test_error_cases() {
    print_test_header "Tests de cas d'erreur"
    
    run_valgrind_test "Commande inexistante" "nonexistent_command"
    run_valgrind_test "Fichier inexistant" "cat /nonexistent/file"
    run_valgrind_test "Permission refusée" "cat /root/.bashrc" 5
    run_valgrind_test "CD vers inexistant" "cd /nonexistent/directory"
    
    echo ""
}

test_stress() {
    print_test_header "Tests de stress"
    
    # Générer des commandes répétitives
    local long_echo_cmd=""
    for i in {1..50}; do
        long_echo_cmd+="echo test$i; "
    done
    
    run_valgrind_test "Nombreuses commandes echo" "$long_echo_cmd" 15
    
    # Test avec beaucoup de pipes
    local pipe_cmd="echo start"
    for i in {1..10}; do
        pipe_cmd+=" | cat"
    done
    
    run_valgrind_test "Nombreux pipes" "$pipe_cmd" 15
    
    echo ""
}

# ==================================================================================
# BONUS TESTS (si implémentés)
# ==================================================================================

test_bonus_features() {
    print_test_header "Tests des fonctionnalités bonus"
    
    if [ "$QUICK" = false ]; then
        run_valgrind_test "AND operator" "echo hello && echo world"
        run_valgrind_test "OR operator" "false || echo world"
        run_valgrind_test "Parentheses" "(echo hello && echo world) || echo failed"
        run_valgrind_test "Wildcards" "echo *.c" 5
    fi
    
    echo ""
}

# ==================================================================================
# RAPPORT ET ANALYSE
# ==================================================================================

generate_report() {
    local report_file="$VALGRIND_LOG_DIR/leak_report.txt"
    
    echo "MINISHELL LEAK TEST REPORT" > "$report_file"
    echo "=========================" >> "$report_file"
    echo "Date: $(date)" >> "$report_file"
    echo "Total tests: $TOTAL_TESTS" >> "$report_file"
    echo "Passed: $PASSED_TESTS" >> "$report_file"
    echo "Failed: $FAILED_TESTS" >> "$report_file"
    echo "" >> "$report_file"
    
    # Analyser les logs pour les fuites les plus communes
    echo "MOST COMMON LEAKS:" >> "$report_file"
    echo "==================" >> "$report_file"
    
    for log in "$VALGRIND_LOG_DIR"/*.log; do
        if [ -f "$log" ]; then
            grep "definitely lost" "$log" >> "$report_file" 2>/dev/null
        fi
    done
    
    print_info "Rapport détaillé généré : $report_file"
}

print_summary() {
    echo ""
    echo -e "${CYAN}================================================================================${NC}"
    echo -e "${CYAN}                                 RÉSUMÉ FINAL${NC}"
    echo -e "${CYAN}================================================================================${NC}"
    echo ""
    
    local success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    
    echo -e "Tests totaux      : ${BLUE}$TOTAL_TESTS${NC}"
    echo -e "Tests réussis     : ${GREEN}$PASSED_TESTS${NC}"
    echo -e "Tests échoués     : ${RED}$FAILED_TESTS${NC}"
    echo -e "Taux de réussite  : ${YELLOW}$success_rate%${NC}"
    echo ""
    
    if [ "$FAILED_TESTS" -eq 0 ]; then
        echo -e "${GREEN}🎉 FÉLICITATIONS ! Aucune fuite de mémoire détectée !${NC}"
    elif [ "$FAILED_TESTS" -le 2 ]; then
        echo -e "${YELLOW}⚠️  Quelques fuites mineures détectées. Vérifiez les logs.${NC}"
    else
        echo -e "${RED}❌ Plusieurs fuites détectées. Révision nécessaire.${NC}"
    fi
    
    echo ""
    echo -e "Logs détaillés dans : ${CYAN}$VALGRIND_LOG_DIR/${NC}"
    
    if [ "$VERBOSE" = true ]; then
        echo ""
        echo -e "${PURPLE}Conseils pour corriger les fuites :${NC}"
        echo "1. Vérifiez que chaque malloc() a son free() correspondant"
        echo "2. Attention aux fuites dans les fonctions de parsing"
        echo "3. Libérez la mémoire en cas d'erreur (exit paths)"
        echo "4. Fermez tous les file descriptors ouverts"
        echo "5. Nettoyez les variables d'environnement copiées"
    fi
    
    echo ""
}

# ==================================================================================
# TRAITEMENT DES ARGUMENTS
# ==================================================================================

parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -q|--quick)
                QUICK=true
                shift
                ;;
            -f|--full)
                FULL=true
                shift
                ;;
            -s|--summary)
                SUMMARY_ONLY=true
                shift
                ;;
            -h|--help)
                echo "Usage: $0 [options]"
                echo "Options:"
                echo "  -v, --verbose    Mode verbeux avec plus de détails"
                echo "  -q, --quick      Tests rapides seulement"
                echo "  -f, --full       Tests complets avec tous les cas"
                echo "  -s, --summary    Affiche seulement le résumé final"
                echo "  -h, --help       Affiche cette aide"
                exit 0
                ;;
            *)
                print_error "Option inconnue: $1"
                exit 1
                ;;
        esac
    done
}

# ==================================================================================
# FONCTION PRINCIPALE
# ==================================================================================

main() {
    parse_arguments "$@"
    
    if [ "$SUMMARY_ONLY" = false ]; then
        print_header
    fi
    
    check_prerequisites
    
    # Exécuter les tests selon les options
    test_basic_commands
    test_redirections
    test_pipes
    test_environment_variables
    test_builtins
    
    if [ "$QUICK" = false ]; then
        test_complex_scenarios
        test_error_cases
        
        if [ "$FULL" = true ]; then
            test_stress
            test_bonus_features
        fi
    fi
    
    generate_report
    print_summary
    
    # Code de sortie basé sur les résultats
    if [ "$FAILED_TESTS" -eq 0 ]; then
        exit 0
    else
        exit 1
    fi
}

# Exécuter le script
main "$@"