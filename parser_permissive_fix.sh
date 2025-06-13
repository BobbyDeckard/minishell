#!/bin/bash

# Correcteur spécialisé pour parser trop permissif
# Focus sur les 4 problèmes critiques de validation syntaxique

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

print_header() {
    echo -e "\n${CYAN}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║ $1${NC}"
    echo -e "${CYAN}╚══════════════════════════════════════════════════════════════╝${NC}"
}

print_problem() {
    echo -e "${RED}❌ PROBLÈME: $1${NC}"
}

print_solution() {
    echo -e "${GREEN}✅ SOLUTION: $1${NC}"
}

print_code() {
    echo -e "${YELLOW}$1${NC}"
}

# Diagnostic précis de votre parser
diagnose_parser() {
    print_header "DIAGNOSTIC DE VOTRE PARSER"
    
    echo "Test en temps réel de votre parser..."
    
    if [ ! -f "./minishell" ]; then
        echo -e "${RED}❌ Compilez d'abord avec 'make'${NC}"
        return 1
    fi
    
    echo -e "\n${BLUE}Test 1: Pipe en début${NC}"
    echo -n "Commande: echo '| cat' | ./minishell → "
    timeout 2 bash -c "echo '| cat' | ./minishell" >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        print_problem "ACCEPTÉ (devrait être rejeté)"
        echo "Votre parser laisse passer '| cat'"
    else
        print_solution "REJETÉ (correct)"
    fi
    
    echo -e "\n${BLUE}Test 2: Pipe en fin${NC}"
    echo -n "Commande: echo 'cat |' | ./minishell → "
    timeout 2 bash -c "echo 'cat |' | ./minishell" >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        print_problem "ACCEPTÉ (devrait être rejeté)"
        echo "Votre parser laisse passer 'cat |'"
    else
        print_solution "REJETÉ (correct)"
    fi
    
    echo -e "\n${BLUE}Test 3: Redirection sans fichier${NC}"
    echo -n "Commande: echo 'cat >' | ./minishell → "
    timeout 2 bash -c "echo 'cat >' | ./minishell" >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        print_problem "ACCEPTÉ (devrait être rejeté)"
        echo "Votre parser laisse passer 'cat >'"
    else
        print_solution "REJETÉ (correct)"
    fi
    
    echo -e "\n${BLUE}Test 4: Guillemets non fermés${NC}"
    echo -n "Commande: echo 'echo \"hello' | ./minishell → "
    timeout 2 bash -c "echo 'echo \"hello' | ./minishell" >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        print_problem "ACCEPTÉ (devrait être rejeté)"
        echo "Votre parser laisse passer les guillemets non fermés"
    else
        print_solution "REJETÉ (correct)"
    fi
}

# Localiser vos fichiers de validation
locate_validation_files() {
    print_header "LOCALISATION DE VOS FICHIERS DE VALIDATION"
    
    echo "Recherche de vos fonctions de validation syntaxique..."
    
    # Trouver le fichier principal de validation
    syntax_file=$(find srcs/parsing/ -name "*.c" -exec grep -l "syntax_is_valid" {} \; 2>/dev/null | head -1)
    
    if [ -n "$syntax_file" ]; then
        echo -e "${GREEN}✅ Fichier principal trouvé: $syntax_file${NC}"
        
        # Analyser les fonctions présentes
        echo -e "\n${BLUE}Fonctions détectées:${NC}"
        
        if grep -q "operator_syntax_is_valid" "$syntax_file"; then
            echo -e "${GREEN}✅ operator_syntax_is_valid() trouvée${NC}"
        else
            echo -e "${RED}❌ operator_syntax_is_valid() manquante${NC}"
        fi
        
        if grep -q "redir_syntax_is_valid" "$syntax_file"; then
            echo -e "${GREEN}✅ redir_syntax_is_valid() trouvée${NC}"
        else
            echo -e "${RED}❌ redir_syntax_is_valid() manquante${NC}"
        fi
        
        if grep -q "quote_syntax_is_valid" "$syntax_file"; then
            echo -e "${GREEN}✅ quote_syntax_is_valid() trouvée${NC}"
        else
            echo -e "${RED}❌ quote_syntax_is_valid() manquante${NC}"
        fi
        
        # Analyser le contenu des fonctions
        analyze_functions "$syntax_file"
        
    else
        echo -e "${RED}❌ Aucun fichier de validation syntaxique trouvé${NC}"
        echo "Recherche dans tous les fichiers de parsing..."
        find srcs/parsing/ -name "*.c" -exec grep -l "syntax" {} \; 2>/dev/null | head -5
    fi
}

# Analyser vos fonctions actuelles
analyze_functions() {
    local file="$1"
    
    print_header "ANALYSE DE VOS FONCTIONS ACTUELLES"
    
    echo -e "${BLUE}Analyse de operator_syntax_is_valid():${NC}"
    local operator_func=$(grep -A 20 "operator_syntax_is_valid" "$file" 2>/dev/null)
    
    if [ -n "$operator_func" ]; then
        echo "Fonction trouvée, analyse..."
        
        # Vérifier si elle contrôle le token précédent
        if echo "$operator_func" | grep -q "prev.*NULL\|!.*prev"; then
            echo -e "${GREEN}✅ Vérifie le token précédent${NC}"
        else
            echo -e "${RED}❌ Ne vérifie PAS le token précédent (PROBLÈME)${NC}"
            echo "   → C'est pourquoi '| cat' passe"
        fi
        
        # Vérifier si elle contrôle le token suivant pour les fins
        if echo "$operator_func" | grep -q "next.*NULL\|!.*next"; then
            echo -e "${GREEN}✅ Vérifie le token suivant${NC}"
        else
            echo -e "${RED}❌ Ne vérifie PAS le token suivant (PROBLÈME)${NC}"
            echo "   → C'est pourquoi 'cat |' passe"
        fi
    else
        echo -e "${RED}❌ Fonction operator_syntax_is_valid() non trouvée ou incomplète${NC}"
    fi
    
    echo -e "\n${BLUE}Analyse de redir_syntax_is_valid():${NC}"
    local redir_func=$(grep -A 15 "redir_syntax_is_valid" "$file" 2>/dev/null)
    
    if [ -n "$redir_func" ]; then
        echo "Fonction trouvée, analyse..."
        
        if echo "$redir_func" | grep -q "next.*NULL\|!.*next"; then
            echo -e "${GREEN}✅ Vérifie le token suivant${NC}"
        else
            echo -e "${RED}❌ Ne vérifie PAS le token suivant (PROBLÈME)${NC}"
            echo "   → C'est pourquoi 'cat >' passe"
        fi
        
        if echo "$redir_func" | grep -q "WORD"; then
            echo -e "${GREEN}✅ Vérifie le type WORD${NC}"
        else
            echo -e "${RED}❌ Ne vérifie PAS le type WORD (PROBLÈME)${NC}"
        fi
    else
        echo -e "${RED}❌ Fonction redir_syntax_is_valid() non trouvée ou incomplète${NC}"
    fi
    
    echo -e "\n${BLUE}Analyse de quote_syntax_is_valid():${NC}"
    local quote_func=$(grep -A 15 "quote_syntax_is_valid" "$file" 2>/dev/null)
    
    if [ -n "$quote_func" ]; then
        echo "Fonction trouvée, analyse..."
        
        if echo "$quote_func" | grep -q "open\|count"; then
            echo -e "${GREEN}✅ Utilise un système de comptage${NC}"
        else
            echo -e "${RED}❌ Ne compte PAS les guillemets (PROBLÈME)${NC}"
            echo "   → C'est pourquoi les guillemets non fermés passent"
        fi
    else
        echo -e "${RED}❌ Fonction quote_syntax_is_valid() non trouvée ou incomplète${NC}"
    fi
}

# Générer les corrections exactes
generate_exact_fixes() {
    print_header "CORRECTIONS EXACTES POUR VOS FONCTIONS"
    
    echo -e "${YELLOW}⚠️  Ces corrections vont rendre votre parser plus STRICT${NC}"
    echo -e "${YELLOW}⚠️  Il rejettera les syntaxes invalides au lieu de les accepter${NC}"
    
    print_code "CORRECTION 1: operator_syntax_is_valid() - Rejeter pipes en début/fin"
    
    cat << 'EOF'

Dans votre fichier de validation syntaxique, remplacez operator_syntax_is_valid() par:

```c
int	operator_syntax_is_valid(t_token **token_list)
{
	t_token	*current;
	t_token	*prev;

	current = *token_list;
	prev = NULL;
	while (current)
	{
		if (current->type == PIPE || current->type == AND || current->type == OR)
		{
			// AJOUT: Vérifier qu'il y a un token AVANT (pas de pipe en début)
			if (!prev)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					current->content);
				return (0);
			}
			
			// AJOUT: Vérifier que le token précédent est valide
			if (prev->type != WORD && prev->type != PAREN_CLOSE)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					current->content);
				return (0);
			}
			
			// AJOUT: Vérifier qu'il y a un token APRÈS (pas de pipe en fin)
			if (!current->next)
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				return (0);
			}
			
			// AJOUT: Vérifier que le token suivant est valide
			if (current->next->type != WORD && current->next->type != PAREN_OPEN)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					current->content);
				return (0);
			}
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
```

EOF

    print_code "CORRECTION 2: redir_syntax_is_valid() - Rejeter redirections sans fichier"
    
    cat << 'EOF'

Remplacez redir_syntax_is_valid() par:

```c
int	redir_syntax_is_valid(t_token **token_list)
{
	t_token	*current;

	current = *token_list;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			// AJOUT: Vérifier qu'il y a un token après la redirection
			if (!current->next)
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				return (0);
			}
			
			// AJOUT: Vérifier que c'est un nom de fichier (WORD)
			if (current->next->type != WORD)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					current->next->content ? current->next->content : "newline");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
```

EOF

    print_code "CORRECTION 3: quote_syntax_is_valid() - Rejeter guillemets non fermés"
    
    cat << 'EOF'

Remplacez quote_syntax_is_valid() par:

```c
int	quote_syntax_is_valid(t_token **token_list)
{
	t_token	*current;
	int		single_open;
	int		double_open;

	current = *token_list;
	single_open = 0;
	double_open = 0;
	
	while (current)
	{
		if (current->type == SINGLE_QUOTE)
			single_open = !single_open;  // Toggle: 0->1->0
		else if (current->type == DOUBLE_QUOTE)
			double_open = !double_open;  // Toggle: 0->1->0
		current = current->next;
	}
	
	// AJOUT: Si des guillemets restent ouverts, c'est une erreur
	if (single_open)
	{
		printf("minishell: syntax error: unterminated quoted string\n");
		return (0);
	}
	if (double_open)
	{
		printf("minishell: syntax error: unterminated quoted string\n");
		return (0);
	}
	return (1);
}
```

EOF
}

# Instructions d'application
application_instructions() {
    print_header "INSTRUCTIONS D'APPLICATION ÉTAPE PAR ÉTAPE"
    
    echo -e "${BLUE}ÉTAPE 1: Localisez votre fichier${NC}"
    print_code "find srcs/parsing/ -name '*.c' -exec grep -l 'syntax_is_valid' {} \\;"
    
    echo -e "\n${BLUE}ÉTAPE 2: Sauvegardez${NC}"
    print_code "cp [votre_fichier_de_validation] [votre_fichier_de_validation].backup"
    
    echo -e "\n${BLUE}ÉTAPE 3: Modifiez vos 3 fonctions${NC}"
    echo "Ouvrez votre fichier de validation et remplacez les 3 fonctions par les versions corrigées ci-dessus"
    
    echo -e "\n${BLUE}ÉTAPE 4: Compilez et testez${NC}"
    print_code "make re"
    print_code "echo '| cat' | ./minishell    # Doit maintenant ÉCHOUER"
    print_code "echo 'cat |' | ./minishell    # Doit maintenant ÉCHOUER"
    print_code "echo 'cat >' | ./minishell    # Doit maintenant ÉCHOUER"
    
    echo -e "\n${BLUE}ÉTAPE 5: Validation complète${NC}"
    print_code "./targeted_fix_tests.sh"
    
    echo -e "\n${GREEN}🎯 OBJECTIF: Passer de 4 FAIL à 4 PASS dans la validation syntaxique${NC}"
}

# Tests de validation post-correction
post_correction_tests() {
    print_header "TESTS POST-CORRECTION"
    
    if [ ! -f "./minishell" ]; then
        echo -e "${RED}❌ Compilez d'abord avec 'make'${NC}"
        return 1
    fi
    
    echo "Vérification que vos corrections fonctionnent..."
    
    local tests_passed=0
    local tests_total=4
    
    # Test 1: Pipe en début
    echo -n "Test 1 - Pipe en début: "
    timeout 2 bash -c "echo '| cat' | ./minishell" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${GREEN}PASS (rejeté correctement)${NC}"
        ((tests_passed++))
    else
        echo -e "${RED}FAIL (encore accepté)${NC}"
    fi
    
    # Test 2: Pipe en fin
    echo -n "Test 2 - Pipe en fin: "
    timeout 2 bash -c "echo 'cat |' | ./minishell" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${GREEN}PASS (rejeté correctement)${NC}"
        ((tests_passed++))
    else
        echo -e "${RED}FAIL (encore accepté)${NC}"
    fi
    
    # Test 3: Redirection sans fichier
    echo -n "Test 3 - Redirection sans fichier: "
    timeout 2 bash -c "echo 'cat >' | ./minishell" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${GREEN}PASS (rejeté correctement)${NC}"
        ((tests_passed++))
    else
        echo -e "${RED}FAIL (encore accepté)${NC}"
    fi
    
    # Test 4: Guillemets non fermés
    echo -n "Test 4 - Guillemets non fermés: "
    timeout 2 bash -c "echo 'echo \"hello' | ./minishell" >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${GREEN}PASS (rejeté correctement)${NC}"
        ((tests_passed++))
    else
        echo -e "${RED}FAIL (encore accepté)${NC}"
    fi
    
    echo -e "\n${BLUE}Résultat: $tests_passed/$tests_total tests passent${NC}"
    
    if [ $tests_passed -eq $tests_total ]; then
        echo -e "${GREEN}🎉 PARFAIT ! Votre parser rejette maintenant les syntaxes invalides !${NC}"
        echo -e "${GREEN}Vous avez corrigé le problème principal de validation syntaxique.${NC}"
    elif [ $tests_passed -ge 2 ]; then
        echo -e "${YELLOW}🔧 EN BONNE VOIE ! $tests_passed corrections sur 4 fonctionnent.${NC}"
        echo -e "${YELLOW}Continuez avec les fonctions restantes.${NC}"
    else
        echo -e "${RED}❌ Les corrections ne sont pas encore appliquées.${NC}"
        echo -e "${RED}Vérifiez que vous avez bien modifié les bonnes fonctions.${NC}"
    fi
    
    # Test de régression
    echo -e "\n${BLUE}Test de régression (syntaxes valides):${NC}"
    timeout 2 bash -c "echo 'echo hello | cat' | ./minishell" >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ Pipe valide fonctionne toujours${NC}"
    else
        echo -e "${RED}❌ ATTENTION: Pipe valide ne fonctionne plus${NC}"
    fi
}

# Menu principal
main() {
    echo -e "${CYAN}"
    echo "╔════════════════════════════════════════════════════════════════╗"
    echo "║                                                                ║"
    echo "║            🔧 CORRECTEUR PARSER TROP PERMISSIF 🔧              ║"
    echo "║                                                                ║"
    echo "║              Rendre votre parser plus STRICT                  ║"
    echo "║                                                                ║"
    echo "╚════════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    
    echo "Votre parser accepte des syntaxes qu'il devrait rejeter."
    echo "Ce script va vous aider à le rendre plus strict."
    
    while true; do
        echo -e "\n${BLUE}Choisissez une action:${NC}"
        echo "1. 🔍 Diagnostiquer mon parser actuel"
        echo "2. 📁 Localiser mes fichiers de validation"
        echo "3. 🔧 Voir les corrections à appliquer"
        echo "4. 📋 Instructions d'application"
        echo "5. ✅ Tester mes corrections"
        echo "0. ❌ Quitter"
        
        echo -n "Votre choix: "
        read -r choice
        
        case $choice in
            1) diagnose_parser ;;
            2) locate_validation_files ;;
            3) generate_exact_fixes ;;
            4) application_instructions ;;
            5) post_correction_tests ;;
            0) 
                echo -e "${GREEN}Bon courage pour vos corrections !${NC}"
                break 
                ;;
            *) 
                echo -e "${RED}❌ Choix invalide${NC}" 
                ;;
        esac
        
        echo -e "\n${BLUE}Appuyez sur Entrée pour continuer...${NC}"
        read -r
    done
}

# Exécution
main "$@"