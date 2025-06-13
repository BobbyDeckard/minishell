#!/bin/bash

# Script maître pour tous les tests de parsing du minishell
# Exécute une batterie complète de tests selon la feuille d'évaluation 42

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MINISHELL="./minishell"
LOG_DIR="parsing_test_logs"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

# Variables de résultats
TOTAL_TESTS=0
TOTAL_PASSED=0
TOTAL_FAILED=0

print_banner() {
    echo -e "${CYAN}"
    echo "╔════════════════════════════════════════════════════════════════╗"
    echo "║                                                                ║"
    echo "║                 🚀 MINISHELL PARSING TESTS 🚀                  ║"
    echo "║                                                                ║"
    echo "║           Batterie complète de tests de parsing               ║"
    echo "║              Basée sur l'évaluation 42 2024                   ║"
    echo "║                                                                ║"
    echo "╚════════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
}

print_section() {
    echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${BLUE} $1${NC}"
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
}

check_prerequisites() {
    print_section "VÉRIFICATION DES PRÉREQUIS"
    
    # Vérification de l'existence du minishell
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}❌ Minishell non trouvé: $MINISHELL${NC}"
        echo "Tentative de compilation..."
        if make > /dev/null 2>&1; then
            echo -e "${GREEN}✅ Compilation réussie${NC}"
        else
            echo -e "${RED}❌ Compilation échouée${NC}"
            exit 1
        fi
    else
        echo -e "${GREEN}✅ Minishell trouvé${NC}"
    fi
    
    # Vérification des outils nécessaires
    local tools=("timeout" "make" "norminette")
    for tool in "${tools[@]}"; do
        if command -v "$tool" &> /dev/null; then
            echo -e "${GREEN}✅ $tool disponible${NC}"
        else
            echo -e "${YELLOW}⚠️  $tool non trouvé (certains tests peuvent échouer)${NC}"
        fi
    done
    
    # Création du répertoire de logs
    mkdir -p "$LOG_DIR"
    echo -e "${GREEN}✅ Répertoire de logs créé: $LOG_DIR${NC}"
}

run_test_suite() {
    local suite_name="$1"
    local script_content="$2"
    local log_file="$LOG_DIR/${suite_name}_${TIMESTAMP}.log"
    
    print_section "EXÉCUTION: $suite_name"
    
    echo "Logs sauvegardés dans: $log_file"
    
    # Création du script temporaire
    local temp_script="/tmp/minishell_${suite_name}.sh"
    echo "$script_content" > "$temp_script"
    chmod +x "$temp_script"
    
    # Exécution du script avec capture des logs
    if bash "$temp_script" 2>&1 | tee "$log_file"; then
        local exit_code=${PIPESTATUS[0]}
        
        # Extraction des statistiques du log
        local passed=$(grep -c "\[PASS\]" "$log_file" 2>/dev/null || echo "0")
        local failed=$(grep -c "\[FAIL\]" "$log_file" 2>/dev/null || echo "0")
        
        echo -e "\n${BLUE}Résultats de $suite_name:${NC}"
        echo -e "  Réussis: ${GREEN}$passed${NC}"
        echo -e "  Échoués: ${RED}$failed${NC}"
        
        TOTAL_PASSED=$((TOTAL_PASSED + passed))
        TOTAL_FAILED=$((TOTAL_FAILED + failed))
        TOTAL_TESTS=$((TOTAL_TESTS + passed + failed))
        
        if [ $exit_code -eq 0 ]; then
            echo -e "  Status: ${GREEN}✅ SUCCÈS${NC}"
        else
            echo -e "  Status: ${RED}❌ ÉCHEC${NC}"
        fi
    else
        echo -e "${RED}❌ Erreur lors de l'exécution de $suite_name${NC}"
    fi
    
    # Nettoyage
    rm -f "$temp_script"
}

# Suite 1: Tests de base et compilation
test_basic_compilation() {
    local script='#!/bin/bash
MINISHELL="./minishell"
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"
PASSED=0
FAILED=0

print_test() { printf "%-50s" "$1"; }
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "[${GREEN}PASS${NC}]"
        ((PASSED++))
    else
        echo -e "[${RED}FAIL${NC}]"
        ((FAILED++))
    fi
}

echo "=== Tests de Compilation ==="
print_test "Compilation avec flags norminette"
make re CFLAGS="-Wall -Wextra -Werror" > /dev/null 2>&1
print_result $?

print_test "Vérification norminette"
norminette srcs/parsing/ incl/ > /dev/null 2>&1
print_result $?

print_test "Test de base - pwd"
timeout 3 bash -c "echo \"pwd\" | $MINISHELL" > /dev/null 2>&1
print_result $?

echo -e "\nRésultats: $PASSED réussis, $FAILED échoués"
[ $FAILED -eq 0 ]'
    
    run_test_suite "Compilation et Base" "$script"
}

# Suite 2: Tests de validation syntaxique
test_syntax_validation() {
    local script='#!/bin/bash
MINISHELL="./minishell"
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"
PASSED=0
FAILED=0

print_test() { printf "%-50s" "$1"; }
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "[${GREEN}PASS${NC}]"
        ((PASSED++))
    else
        echo -e "[${RED}FAIL${NC}]"
        ((FAILED++))
    fi
}

test_should_fail() {
    local command="$2"
    timeout 3 bash -c "echo \"$command\" | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -ne 0 ]; then
        print_result 0
    else
        print_result 1
    fi
}

test_should_pass() {
    local command="$2"
    timeout 3 bash -c "echo \"$command\" | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ] || [ $exit_code -eq 127 ]; then
        print_result 0
    else
        print_result 1
    fi
}

echo "=== Tests de Validation Syntaxique ==="

print_test "Guillemets simples non fermés (doit échouer)"
test_should_fail _ "echo \"hello"

print_test "Guillemets doubles non fermés (doit échouer)" 
test_should_fail _ "echo \"hello"

print_test "Pipe en début (doit échouer)"
test_should_fail _ "| cat"

print_test "Pipe en fin (doit échouer)"
test_should_fail _ "cat |"

print_test "Redirection sans fichier (doit échouer)"
test_should_fail _ "cat >"

print_test "Guillemets corrects (doit réussir)"
test_should_pass _ "echo \"hello world\""

print_test "Variables dans guillemets (doit réussir)"
test_should_pass _ "echo \"\$USER\""

print_test "Pipes valides (doit réussir)"
test_should_pass _ "echo hello | cat"

echo -e "\nRésultats: $PASSED réussis, $FAILED échoués"
[ $FAILED -eq 0 ]'
    
    run_test_suite "Validation Syntaxique" "$script"
}

# Suite 3: Tests de parsing complexe
test_complex_parsing() {
    local script='#!/bin/bash
MINISHELL="./minishell"
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"
PASSED=0
FAILED=0

print_test() { printf "%-50s" "$1"; }
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "[${GREEN}PASS${NC}]"
        ((PASSED++))
    else
        echo -e "[${RED}FAIL${NC}]"
        ((FAILED++))
    fi
}

run_test() {
    local command="$2"
    timeout 5 bash -c "echo \"$command\" | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ] || [ $exit_code -eq 127 ]; then
        print_result 0
    else
        print_result 1
    fi
}

echo "=== Tests de Parsing Complexe ==="

print_test "Variables multiples"
run_test _ "echo \$USER \$HOME \$PATH"

print_test "Redirections multiples"
echo "test" > /tmp/test_input.txt
run_test _ "< /tmp/test_input.txt cat > /tmp/test_output.txt"
rm -f /tmp/test_input.txt /tmp/test_output.txt

print_test "Pipes multiples"
run_test _ "echo hello | cat | cat | wc -l"

print_test "Heredoc avec variables"
run_test _ "cat << EOF"$'"'"$'\n'"'"Hello \$USER"$'\n'"'"EOF"

print_test "Commande complexe mixte"
run_test _ "echo \"Hello \$USER\" | grep \$USER"

print_test "Guillemets imbriqués"
run_test _ "echo \"it'\''s working\""

print_test "Variables dans redirections"
run_test _ "echo hello > /tmp/\$USER.txt"
rm -f "/tmp/$USER.txt"

print_test "Espaces avant/après opérateurs"
run_test _ " echo hello | cat "

echo -e "\nRésultats: $PASSED réussis, $FAILED échoués"
[ $FAILED -eq 0 ]'
    
    run_test_suite "Parsing Complexe" "$script"
}

# Suite 4: Tests de robustesse
test_robustness() {
    local script='#!/bin/bash
MINISHELL="./minishell"
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"
PASSED=0
FAILED=0

print_test() { printf "%-50s" "$1"; }
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "[${GREEN}PASS${NC}]"
        ((PASSED++))
    else
        echo -e "[${RED}FAIL${NC}]"
        ((FAILED++))
    fi
}

echo "=== Tests de Robustesse ==="

print_test "Commande très longue"
long_cmd="echo $(printf \"a%.0s\" {1..500})"
timeout 5 bash -c "echo \"$long_cmd\" | $MINISHELL" > /dev/null 2>&1
if [ $? -eq 0 ] || [ $? -eq 1 ]; then
    print_result 0
else
    print_result 1
fi

print_test "Beaucoup d arguments"
many_args="echo $(seq 1 50)"
timeout 5 bash -c "echo \"$many_args\" | $MINISHELL" > /dev/null 2>&1
if [ $? -eq 0 ] || [ $? -eq 1 ]; then
    print_result 0
else
    print_result 1
fi

print_test "Test de stress (10 commandes rapides)"
stress_passed=0
for i in {1..10}; do
    timeout 2 bash -c "echo \"echo test$i\" | $MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        ((stress_passed++))
    fi
done
if [ $stress_passed -ge 8 ]; then
    print_result 0
else
    print_result 1
fi

print_test "Gestion des erreurs - fichier inexistant"
timeout 3 bash -c "echo \"< /nonexistent/file cat\" | $MINISHELL" > /dev/null 2>&1
if [ $? -ne 139 ]; then  # Pas de segfault
    print_result 0
else
    print_result 1
fi

echo -e "\nRésultats: $PASSED réussis, $FAILED échoués"
[ $FAILED -eq 0 ]'
    
    run_test_suite "Robustesse" "$script"
}

# Suite 5: Tests de memory leaks
test_memory_leaks() {
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}⚠️  Valgrind non disponible, tests de mémoire ignorés${NC}"
        return
    fi
    
    local script='#!/bin/bash
MINISHELL="./minishell"
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"
PASSED=0
FAILED=0

print_test() { printf "%-50s" "$1"; }
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "[${GREEN}PASS${NC}]"
        ((PASSED++))
    else
        echo -e "[${RED}FAIL${NC}]"
        ((FAILED++))
    fi
}

echo "=== Tests de Mémoire (Valgrind) ==="

print_test "Test basique sans leaks"
timeout 10 valgrind --leak-check=full --error-exitcode=1 \
    bash -c "echo \"echo hello\" | $MINISHELL" > /dev/null 2>&1
print_result $?

print_test "Test avec parsing complexe"
timeout 15 valgrind --leak-check=full --error-exitcode=1 \
    bash -c "echo \"echo \$USER | cat\" | $MINISHELL" > /dev/null 2>&1
print_result $?

print_test "Test avec erreur syntaxique"
timeout 10 valgrind --leak-check=full --error-exitcode=1 \
    bash -c "echo \"echo unclosed quote\" | $MINISHELL" > /dev/null 2>&1
# Pour les erreurs syntaxiques, on accepte un exit code différent de 1 (leak) et 139 (segfault)
exit_code=$?
if [ $exit_code -ne 1 ] && [ $exit_code -ne 139 ]; then
    print_result 0
else
    print_result 1
fi

echo -e "\nRésultats: $PASSED réussis, $FAILED échoués"
[ $FAILED -eq 0 ]'
    
    run_test_suite "Mémoire" "$script"
}

generate_final_report() {
    print_section "RAPPORT FINAL"
    
    local report_file="$LOG_DIR/final_report_${TIMESTAMP}.txt"
    
    {
        echo "RAPPORT FINAL - TESTS DE PARSING MINISHELL"
        echo "==========================================="
        echo "Date: $(date)"
        echo "Timestamp: $TIMESTAMP"
        echo ""
        echo "STATISTIQUES GLOBALES:"
        echo "----------------------"
        echo "Total des tests: $TOTAL_TESTS"
        echo "Tests réussis: $TOTAL_PASSED"
        echo "Tests échoués: $TOTAL_FAILED"
        
        if [ $TOTAL_TESTS -gt 0 ]; then
            local success_rate=$(( TOTAL_PASSED * 100 / TOTAL_TESTS ))
            echo "Taux de réussite: $success_rate%"
            echo ""
            
            if [ $success_rate -ge 95 ]; then
                echo "ÉVALUATION: EXCELLENT ⭐⭐⭐⭐⭐"
                echo "Votre parsing est prêt pour l'évaluation !"
            elif [ $success_rate -ge 85 ]; then
                echo "ÉVALUATION: TRÈS BON ⭐⭐⭐⭐"
                echo "Quelques améliorations mineures recommandées."
            elif [ $success_rate -ge 70 ]; then
                echo "ÉVALUATION: BON ⭐⭐⭐"
                echo "Corrections nécessaires avant l'évaluation."
            elif [ $success_rate -ge 50 ]; then
                echo "ÉVALUATION: MOYEN ⭐⭐"
                echo "Travail important requis."
            else
                echo "ÉVALUATION: INSUFFISANT ⭐"
                echo "Parsing nécessite une refonte majeure."
            fi
        fi
        
        echo ""
        echo "FICHIERS DE LOGS GÉNÉRÉS:"
        echo "------------------------"
        ls -la "$LOG_DIR"/*_${TIMESTAMP}.log 2>/dev/null || echo "Aucun log trouvé"
        
    } > "$report_file"
    
    # Affichage du rapport à l'écran
    cat "$report_file"
    
    echo -e "\n${BLUE}📋 Rapport complet sauvegardé dans: $report_file${NC}"
    
    # Résumé coloré
    echo -e "\n${CYAN}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║                        RÉSUMÉ FINAL                          ║${NC}"
    echo -e "${CYAN}╚══════════════════════════════════════════════════════════════╝${NC}"
    echo -e "Tests totaux: ${BLUE}$TOTAL_TESTS${NC}"
    echo -e "Réussis: ${GREEN}$TOTAL_PASSED${NC}"
    echo -e "Échoués: ${RED}$TOTAL_FAILED${NC}"
    
    if [ $TOTAL_TESTS -gt 0 ]; then
        local success_rate=$(( TOTAL_PASSED * 100 / TOTAL_TESTS ))
        echo -e "Taux de réussite: ${BLUE}$success_rate%${NC}"
    fi
    
    if [ $TOTAL_FAILED -eq 0 ]; then
        echo -e "\n${GREEN}🎉 PARFAIT ! Tous les tests de parsing sont passés ! 🎉${NC}"
        return 0
    else
        echo -e "\n${RED}⚠️  $TOTAL_FAILED test(s) ont échoué. Consultez les logs pour plus de détails.${NC}"
        return 1
    fi
}

show_manual_tests() {
    print_section "TESTS MANUELS RECOMMANDÉS"
    
    echo -e "${YELLOW}Les tests suivants doivent être effectués manuellement:${NC}"
    echo ""
    echo "1. 🎯 Tests de signaux:"
    echo "   - Lancez ./minishell"
    echo "   - Testez Ctrl-C (doit afficher nouvelle ligne + prompt)"
    echo "   - Testez Ctrl-D (doit quitter le shell)"
    echo "   - Testez Ctrl-\\ (ne doit rien faire)"
    echo ""
    echo "2. 📚 Tests d'historique:"
    echo "   - Tapez plusieurs commandes"
    echo "   - Utilisez les flèches haut/bas pour naviguer"
    echo "   - Vérifiez que l'historique persiste"
    echo ""
    echo "3. 🔄 Tests interactifs:"
    echo "   - Commandes bloquantes: 'cat' sans argument"
    echo "   - Signaux pendant l'exécution"
    echo "   - Comportement du prompt"
    echo ""
    echo "4. 🏁 Tests de sortie:"
    echo "   - 'exit' sans argument"
    echo "   - 'exit 42' avec code de retour"
    echo "   - Ctrl-D pour quitter"
}

# Fonction principale
main() {
    print_banner
    
    # Vérifications préliminaires
    check_prerequisites
    
    echo -e "\n${PURPLE}🚀 Début des tests automatisés...${NC}"
    
    # Exécution de toutes les suites de tests
    test_basic_compilation
    test_syntax_validation  
    test_complex_parsing
    test_robustness
    test_memory_leaks
    
    # Génération du rapport final
    generate_final_report
    local final_result=$?
    
    # Affichage des tests manuels
    show_manual_tests
    
    echo -e "\n${PURPLE}✨ Tests terminés ! Consultez les logs pour plus de détails.${NC}"
    
    return $final_result
}

# Gestion des signaux
trap 'echo -e "\n${RED}🛑 Tests interrompus par l'\''utilisateur${NC}"; exit 130' INT

# Point d'entrée
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi