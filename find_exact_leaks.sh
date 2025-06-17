#!/bin/bash

# ==================================================================================
# DÉTECTEUR AUTOMATIQUE DE FUITES - LOCALISATION EXACTE
# ==================================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}🔍 DÉTECTEUR AUTOMATIQUE DE FUITES MÉMOIRE${NC}"
echo "=============================================="
echo ""

# Test avec diagnostic complet
echo -e "${BLUE}[ANALYSE]${NC} Test en cours..."
echo "echo hello world" | timeout 10 valgrind \
    --leak-check=full \
    --show-leak-kinds=definite \
    --track-origins=yes \
    --num-callers=30 \
    --error-limit=no \
    ./minishell 2> precise_leak_analysis.log

echo ""

# Extraire les informations de fuites
if [ -f "precise_leak_analysis.log" ]; then
    echo -e "${YELLOW}📊 RÉSULTATS D'ANALYSE${NC}"
    echo "========================"
    
    # Résumé des fuites
    echo -e "${CYAN}💧 Résumé des fuites:${NC}"
    grep -A 6 "LEAK SUMMARY" precise_leak_analysis.log
    
    echo ""
    
    # Fuites détaillées
    echo -e "${CYAN}🎯 Localisation exacte des fuites:${NC}"
    grep -A 20 "definitely lost" precise_leak_analysis.log | while read line; do
        if [[ "$line" =~ "definitely lost" ]]; then
            echo -e "${RED}❌ FUITE DÉTECTÉE:${NC} $line"
        elif [[ "$line" =~ "at 0x" ]]; then
            # Extraire fonction et fichier
            func=$(echo "$line" | sed 's/.*at 0x[0-9A-F]*: \([^(]*\).*/\1/')
            location=$(echo "$line" | sed 's/.*(\([^)]*\)).*/\1/')
            if [[ "$func" =~ (tokenize|parse|create_ast|create_token|malloc|free) ]]; then
                echo -e "  ${YELLOW}📍 Dans:${NC} $func ${BLUE}($location)${NC}"
            fi
        fi
    done
    
    echo ""
    
    # Recommandations spécifiques
    echo -e "${CYAN}💡 RECOMMANDATIONS SPÉCIFIQUES:${NC}"
    
    if grep -q "tokenize_input.*retrieve.c:38" precise_leak_analysis.log; then
        echo -e "${RED}❌ PROBLÈME:${NC} tokenize_input() ne libère pas le tableau de pointeurs"
        echo -e "${GREEN}✅ SOLUTION:${NC} Ajoutez free(token_list) après free_token_list()"
        echo ""
    fi
    
    if grep -q "parse_input" precise_leak_analysis.log; then
        echo -e "${RED}❌ PROBLÈME:${NC} parse_input() ne libère pas complètement les tokens"
        echo -e "${GREEN}✅ SOLUTION:${NC} Vérifiez la libération après create_ast()"
        echo ""
    fi
    
    if grep -q "create_.*node" precise_leak_analysis.log; then
        echo -e "${RED}❌ PROBLÈME:${NC} Fuite dans la création de nœuds AST"
        echo -e "${GREEN}✅ SOLUTION:${NC} Vérifiez free_ast() et gestion d'erreurs"
        echo ""
    fi
    
    # Code à ajouter exactement
    echo -e "${CYAN}🔧 CODE À CORRIGER:${NC}"
    echo "==================="
    
    if grep -q "tokenize_input" precise_leak_analysis.log; then
        echo -e "${YELLOW}Dans parse.c, fonction parse_input():${NC}"
        echo ""
        echo "// Après create_ast():"
        echo "ast_root = create_ast(token_list);"
        echo ""
        echo "// AJOUTEZ CES LIGNES:"
        echo -e "${GREEN}if (token_list) {"
        echo "    if (*token_list)"
        echo "        free_token_list(token_list);"
        echo "    free(token_list);  // ← CRUCIAL: Libérer le tableau"
        echo "    token_list = NULL;"
        echo "}${NC}"
        echo ""
    fi
    
    # Vérifications supplémentaires
    echo -e "${CYAN}🔍 VÉRIFICATIONS SUPPLÉMENTAIRES:${NC}"
    echo "================================="
    
    echo "1. Vérifiez que free_token_list() libère bien tout:"
    echo "   grep -A 20 \"void.*free_token_list\" srcs/parsing/tokens.c"
    echo ""
    
    echo "2. Vérifiez l'allocation dans tokenize_input():"
    echo "   grep -A 10 -B 5 \"malloc.*token_list\" srcs/parsing/retrieve.c"
    echo ""
    
    echo "3. Testez rapidement:"
    echo "   echo \"test\" | valgrind --leak-check=yes ./minishell 2>&1 | grep \"definitely lost\""
    echo ""
    
    # Ligne par ligne des fuites
    echo -e "${CYAN}📋 STACK TRACES DÉTAILLÉS:${NC}"
    echo "==========================="
    grep -A 25 "definitely lost" precise_leak_analysis.log | \
    grep -E "(at 0x|by 0x)" | \
    while read line; do
        if [[ "$line" =~ \(([^:]*):([0-9]+)\) ]]; then
            file="${BASH_REMATCH[1]}"
            lineno="${BASH_REMATCH[2]}"
            func=$(echo "$line" | sed 's/.*: \([^(]*\).*/\1/')
            echo -e "${BLUE}📄 Fichier:${NC} $file ${BLUE}Ligne:${NC} $lineno ${BLUE}Fonction:${NC} $func"
        fi
    done
    
    echo ""
    echo -e "${GREEN}📄 Log complet sauvé dans: precise_leak_analysis.log${NC}"
    
else
    echo -e "${RED}❌ Erreur: Impossible de créer le log d'analyse${NC}"
    exit 1
fi

echo ""
echo -e "${CYAN}🎯 PROCHAINES ÉTAPES:${NC}"
echo "1. Appliquez les corrections suggérées ci-dessus"
echo "2. Recompilez: make"
echo "3. Testez: ./find_exact_leaks.sh"
echo "4. Répétez jusqu'à 0 fuites"