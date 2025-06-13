#!/bin/bash

# Tests ciblés pour valider les corrections du parsing
# Se concentre sur les 9 tests qui ont échoué

MINISHELL="./minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0

print_header() {
    echo -e "\n${BLUE}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║ $1${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════════════════════╝${NC}"
}

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
        if [ -n "$2" ]; then
            echo "    $2"
        fi
    fi
}

# Test 1: Norminette
test_norminette() {
    print_header "TEST 1: NORMINETTE (CRITIQUE)"
    
    print_test "Vérification norminette parsing"
    norminette srcs/parsing/ incl/parsing.h > /tmp/norm_result 2>&1
    local norm_errors=$(grep -c "Error" /tmp/norm_result 2>/dev/null || echo "0")
    
    if [ "$norm_errors" -eq 0 ]; then
        print_result 0
    else
        print_result 1 "$norm_errors erreur(s) norminette détectée(s)"
        echo "Détails dans /tmp/norm_result"
    fi
    
    rm -f /tmp/norm_result
}

# Test 2: Validation syntaxique (le plus critique)
test_syntax_validation() {
    print_header "TEST 2: VALIDATION SYNTAXIQUE (CRITIQUE)"
    
    # Test: Pipe en début (doit ÉCHOUER)
    print_test "Pipe en début doit échouer"
    timeout 3 bash -c "echo '| cat' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
        print_result 0
    else
        print_result 1 "Le parser accepte '| cat' (devrait rejeter)"
    fi
    
    # Test: Pipe en fin (doit ÉCHOUER)
    print_test "Pipe en fin doit échouer"
    timeout 3 bash -c "echo 'cat |' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
        print_result 0
    else
        print_result 1 "Le parser accepte 'cat |' (devrait rejeter)"
    fi
    
    # Test: Redirection sans fichier (doit ÉCHOUER)
    print_test "Redirection sans fichier doit échouer"
    timeout 3 bash -c "echo 'cat >' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
        print_result 0
    else
        print_result 1 "Le parser accepte 'cat >' (devrait rejeter)"
    fi
    
    # Test: Guillemets non fermés (doit ÉCHOUER)
    print_test "Guillemets non fermés doit échouer"
    timeout 3 bash -c "echo 'echo \"hello' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
        print_result 0
    else
        print_result 1 "Le parser accepte guillemets non fermés (devrait rejeter)"
    fi
}

# Test 3: Codes de retour corrects
test_return_codes() {
    print_header "TEST 3: CODES DE RETOUR"
    
    # Test: Commande inexistante doit retourner 127
    print_test "Commande inexistante retourne 127"
    timeout 3 bash -c "echo 'commandeinexistante123' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -eq 127 ]; then
        print_result 0
    else
        print_result 1 "Exit code: $exit_code (attendu: 127)"
    fi
    
    # Test: Fichier inexistant en entrée
    print_test "Fichier inexistant en redirection"
    timeout 3 bash -c "echo '< /nonexistent/file12345 cat' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -eq 1 ] || [ $exit_code -eq 2 ]; then
        print_result 0
    else
        print_result 1 "Exit code: $exit_code (attendu: 1 ou 2)"
    fi
    
    # Test: Commande inexistante dans pipe
    print_test "Commande inexistante dans pipe retourne 127"
    timeout 3 bash -c "echo 'commandeinexistante123 | cat' | $MINISHELL" > /dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -eq 127 ]; then
        print_result 0
    else
        print_result 1 "Exit code: $exit_code (attendu: 127)"
    fi
}

# Test 4: Caractères spéciaux dans guillemets simples
test_special_chars() {
    print_header "TEST 4: CARACTÈRES SPÉCIAUX"
    
    print_test "Caractères spéciaux dans guillemets simples"
    # Créer un fichier temporaire pour capturer la sortie
    timeout 3 bash -c "echo \"echo '| > < & ;'\" | $MINISHELL" > /tmp/special_chars_output 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        # Vérifier que la sortie contient les caractères spéciaux
        if grep -q "| > < & ;" /tmp/special_chars_output; then
            print_result 0
        else
            print_result 1 "Sortie incorrecte (caractères spéciaux non préservés)"
        fi
    else
        print_result 1 "Échec d'exécution (exit code: $exit_code)"
    fi
    
    rm -f /tmp/special_chars_output
}

# Test 5: Tests de régression (s'assurer que les corrections n'ont pas cassé le reste)
test_regression() {
    print_header "TEST 5: RÉGRESSION (VALIDATION)"
    
    # Tests qui marchaient avant et doivent continuer à marcher
    local regression_tests=(
        "pwd:Commande pwd"
        "echo hello:Echo simple" 
        "echo \$USER:Variable USER"
        "echo 'hello world':Guillemets simples valides"
        "echo \"hello world\":Guillemets doubles valides"
        "echo hello | cat:Pipe simple valide"
    )
    
    for test_case in "${regression_tests[@]}"; do
        local command="${test_case%%:*}"
        local description="${test_case##*:}"
        
        print_test "$description"
        timeout 3 bash -c "echo '$command' | $MINISHELL" > /dev/null 2>&1
        local exit_code=$?
        
        if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ] || [ $exit_code -eq 127 ]; then
            print_result 0
        else
            print_result 1 "Régression détectée (exit code: $exit_code)"
        fi
    done
}

# Test 6: Validation complète avec cas complexes
test_complex_cases() {
    print_header "TEST 6: CAS COMPLEXES"
    
    print_test "Commande complexe avec variables et guillemets"
    timeout 5 bash -c "echo 'echo \"Hello \$USER\" | grep \$USER' | $MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ] || [ $? -eq 1 ]; then
        print_result 0
    else
        print_result 1 "Échec sur cas complexe"
    fi
    
    print_test "Redirections multiples valides"
    echo "test input" > /tmp/test_input_file
    timeout 3 bash -c "echo '< /tmp/test_input_file cat > /tmp/test_output_file' | $MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ] && [ -f /tmp/test_output_file ]; then
        print_result 0
    else
        print_result 1 "Échec redirections multiples"
    fi
    rm -f /tmp/test_input_file /tmp/test_output_file
    
    print_test "Heredoc simple"
    timeout 5 bash -c "echo 'cat << EOF
hello world
EOF' | $MINISHELL" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        print_result 0
    else
        print_result 1 "Échec heredoc simple"
    fi
}

# Fonction de résumé et recommandations
print_summary() {
    local total=$((PASSED + FAILED))
    
    print_header "RÉSUMÉ DES CORRECTIONS"
    
    echo -e "Tests ciblés exécutés: ${BLUE}$total${NC}"
    echo -e "Corrections validées: ${GREEN}$PASSED${NC}"
    echo -e "Problèmes restants: ${RED}$FAILED${NC}"
    
    if [ $total -gt 0 ]; then
        local success_rate=$(( PASSED * 100 / total ))
        echo -e "Taux de correction: ${BLUE}$success_rate%${NC}"
        
        echo -e "\n${BLUE}═══ ÉVALUATION DES CORRECTIONS ═══${NC}"
        
        if [ $success_rate -eq 100 ]; then
            echo -e "${GREEN}🎉 PARFAIT ! Toutes les corrections sont validées !${NC}"
            echo -e "Votre parsing est maintenant prêt pour l'évaluation finale."
        elif [ $success_rate -ge 80 ]; then
            echo -e "${YELLOW}🔧 BIEN ! La plupart des corrections sont validées.${NC}"
            echo -e "Quelques ajustements mineurs restent nécessaires."
        else
            echo -e "${RED}⚠️ ATTENTION ! Corrections importantes encore nécessaires.${NC}"
            echo -e "Consultez les détails ci-dessus pour les problèmes restants."
        fi
    fi
    
    if [ $FAILED -gt 0 ]; then
        echo -e "\n${YELLOW}📋 PROCHAINES ÉTAPES RECOMMANDÉES :${NC}"
        
        if grep -q "norminette" <<< "$output" 2>/dev/null; then
            echo "1. 🔍 Corriger les erreurs norminette (priorité absolue)"
        fi
        
        if [ $FAILED -ge 3 ]; then
            echo "2. 🎯 Se concentrer sur la validation syntaxique"
            echo "3. 🔧 Vérifier les codes de retour des processus"
        fi
        
        echo "4. 🧪 Re-tester avec la batterie complète"
        echo "5. 📋 Valider manuellement les cas edge"
    fi
    
    return $([ $FAILED -eq 0 ] && echo 0 || echo 1)
}

# Fonction principale
main() {
    echo -e "${BLUE}"
    echo "╔══════════════════════════════════════════════════════════════╗"
    echo "║                VALIDATION DES CORRECTIONS                   ║"
    echo "║                                                              ║"
    echo "║         Tests ciblés sur les 9 échecs identifiés            ║"
    echo "╚══════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}ERREUR: $MINISHELL non trouvé${NC}"
        echo "Compilez d'abord le projet avec 'make'"
        exit 1
    fi
    
    # Exécution des tests ciblés
    test_norminette
    test_syntax_validation
    test_return_codes
    test_special_chars
    test_regression
    test_complex_cases
    
    # Résumé et recommandations
    print_summary
    local result=$?
    
    echo -e "\n${BLUE}💡 TIP: Utilisez './test_parsing.sh' pour les tests complets après corrections${NC}"
    
    exit $result
}

# Gestion des signaux
trap 'echo -e "\n${RED}Tests interrompus${NC}"; exit 130' INT

# Exécution
main "$@"