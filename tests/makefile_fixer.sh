#!/bin/bash

# ============================================================================
# SCRIPT DE CORRECTION ET VÉRIFICATION DU MAKEFILE
# ============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

MAKEFILE="Makefile"
BACKUP="Makefile.backup"

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}    CORRECTION DU MAKEFILE DE TEST${NC}"
echo -e "${BLUE}============================================${NC}"

# Vérifier que le Makefile existe
if [ ! -f "$MAKEFILE" ]; then
    echo -e "${RED}❌ Erreur: $MAKEFILE non trouvé${NC}"
    echo "Veuillez d'abord copier le Makefile depuis les artifacts Claude"
    exit 1
fi

# Créer une sauvegarde
echo -e "${YELLOW}📋 Création d'une sauvegarde...${NC}"
cp "$MAKEFILE" "$BACKUP"
echo -e "${GREEN}✅ Sauvegarde créée: $BACKUP${NC}"

# Fonction pour corriger les tabulations
fix_tabs() {
    echo -e "${YELLOW}🔧 Correction des tabulations...${NC}"
    
    # Sauvegarder le fichier original avec un nom temporaire
    cp "$MAKEFILE" "${MAKEFILE}.temp"
    
    # Remplacer les espaces en début de ligne par des tabulations pour les commandes
    # Les lignes qui commencent par des espaces suivant une ligne avec ":" sont des commandes
    awk '
    /^[^[:space:]].*:/ { 
        target_line = 1
        print $0
        next
    }
    target_line && /^[[:space:]]/ {
        # Remplacer les espaces de début par une tabulation
        gsub(/^[[:space:]]+/, "\t")
        print $0
        next
    }
    /^$/ {
        target_line = 0
        print $0
        next
    }
    /^[^[:space:]]/ {
        target_line = 0
        print $0
        next
    }
    {
        print $0
    }
    ' "${MAKEFILE}.temp" > "$MAKEFILE"
    
    rm "${MAKEFILE}.temp"
    echo -e "${GREEN}✅ Tabulations corrigées${NC}"
}

# Fonction pour vérifier la syntaxe
check_syntax() {
    echo -e "${YELLOW}🔍 Vérification de la syntaxe...${NC}"
    
    if make -n -f "$MAKEFILE" info >/dev/null 2>&1; then
        echo -e "${GREEN}✅ Syntaxe Makefile correcte${NC}"
        return 0
    else
        echo -e "${RED}❌ Erreurs de syntaxe détectées${NC}"
        echo -e "${YELLOW}Détails de l'erreur:${NC}"
        make -n -f "$MAKEFILE" info 2>&1 | head -10
        return 1
    fi
}

# Fonction pour corriger les problèmes courants
fix_common_issues() {
    echo -e "${YELLOW}🔧 Correction des problèmes courants...${NC}"
    
    # Corriger les lignes de continuation incorrectes
    sed -i 's/\\$/\\/g' "$MAKEFILE"
    
    # S'assurer que les heredocs sont correctement formatés
    sed -i '/cat > .*<< .EOF./,/^EOF$/s/^[[:space:]]\+/\t/' "$MAKEFILE"
    
    echo -e "${GREEN}✅ Problèmes courants corrigés${NC}"
}

# Fonction pour créer un Makefile simplifié en cas d'échec
create_simple_makefile() {
    echo -e "${YELLOW}🛠️  Création d'un Makefile simplifié...${NC}"
    
    cat > "${MAKEFILE}.simple" << 'EOF'
# **************************************************************************** #
#                         MAKEFILE SIMPLIFIÉ POUR TESTS                       #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
INCLUDES = -I../incl -I../libft

# Sources nécessaires (adaptez selon votre structure)
PARSING_SOURCES = ../srcs/parsing/retrieve.c \
                  ../srcs/parsing/tokenize.c \
                  ../srcs/parsing/tokens.c \
                  ../srcs/parsing/syntax_check/syntax_check.c \
                  ../srcs/parsing/ast/ast_builder.c

LIBFT = ../libft/libft.a

# Tests unitaires
test_parsing_unit: parsing_unit_tests.c $(PARSING_SOURCES) $(LIBFT)
	@echo "Compilation des tests unitaires..."
	$(CC) $(CFLAGS) $(INCLUDES) parsing_unit_tests.c $(PARSING_SOURCES) $(LIBFT) -o test_parsing_unit

# Tests d'intégration
test-integration:
	@echo "Lancement des tests d'intégration..."
	@chmod +x test_parsing.sh
	@./test_parsing.sh

# Test principal
test: test_parsing_unit
	@echo "Exécution des tests..."
	@./test_parsing_unit

# Nettoyage
clean:
	@rm -f test_parsing_unit

.PHONY: test test-integration clean
EOF

    echo -e "${GREEN}✅ Makefile simplifié créé: ${MAKEFILE}.simple${NC}"
    echo -e "${YELLOW}Pour l'utiliser: mv ${MAKEFILE}.simple ${MAKEFILE}${NC}"
}

# Fonction principale
main() {
    echo -e "${BLUE}Début de la correction...${NC}\n"
    
    # Étape 1: Corriger les tabulations
    fix_tabs
    
    # Étape 2: Corriger les problèmes courants
    fix_common_issues
    
    # Étape 3: Vérifier la syntaxe
    if check_syntax; then
        echo -e "\n${GREEN}🎉 Makefile corrigé avec succès !${NC}"
        echo -e "${GREEN}Vous pouvez maintenant utiliser:${NC}"
        echo -e "  make test-unit"
        echo -e "  make test-integration"
        echo -e "  make test-all"
    else
        echo -e "\n${RED}❌ Impossible de corriger automatiquement${NC}"
        echo -e "${YELLOW}Options:${NC}"
        echo -e "1. ${YELLOW}Utiliser le Makefile simplifié:${NC}"
        echo -e "   mv ${MAKEFILE}.simple ${MAKEFILE}"
        echo -e "2. ${YELLOW}Restaurer la sauvegarde et corriger manuellement:${NC}"
        echo -e "   mv $BACKUP $MAKEFILE"
        echo -e "3. ${YELLOW}Recréer le Makefile depuis les artifacts Claude${NC}"
        
        # Créer le Makefile simplifié comme alternative
        create_simple_makefile
    fi
    
    echo -e "\n${BLUE}============================================${NC}"
    echo -e "${BLUE}Correction terminée${NC}"
    echo -e "${BLUE}============================================${NC}"
}

# Fonction d'aide
show_help() {
    echo "Usage: $0 [option]"
    echo ""
    echo "Options:"
    echo "  --help, -h    Afficher cette aide"
    echo "  --check       Vérifier uniquement (sans corriger)"
    echo "  --simple      Créer directement le Makefile simplifié"
    echo "  --restore     Restaurer depuis la sauvegarde"
    echo ""
    echo "Sans option: correction automatique complète"
}

# Gestion des arguments
case "${1:-}" in
    --help|-h)
        show_help
        exit 0
        ;;
    --check)
        check_syntax
        exit $?
        ;;
    --simple)
        create_simple_makefile
        echo "Pour l'utiliser: mv ${MAKEFILE}.simple ${MAKEFILE}"
        exit 0
        ;;
    --restore)
        if [ -f "$BACKUP" ]; then
            mv "$BACKUP" "$MAKEFILE"
            echo -e "${GREEN}✅ Makefile restauré depuis la sauvegarde${NC}"
        else
            echo -e "${RED}❌ Aucune sauvegarde trouvée${NC}"
            exit 1
        fi
        exit 0
        ;;
    "")
        main
        ;;
    *)
        echo "Option inconnue: $1"
        echo "Utilisez --help pour voir les options disponibles"
        exit 1
        ;;
esac