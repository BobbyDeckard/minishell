#!/bin/bash
echo "=== TESTS MINISHELL ==="

# Test 1: Compilation
echo "Test 1: Compilation des tests..."
gcc -Wall -Wextra -Werror -g3 -I../incl -I../libft \
    parsing_unit_tests.c \
    ../srcs/parsing/*.c \
    ../libft/libft.a \
    -o test_unit

# Test 2: Exécution
if [ -f test_unit ]; then
    echo "Test 2: Exécution des tests unitaires..."
    ./test_unit
else
    echo "Erreur: Compilation échouée"
fi

# Test 3: Test du shell
echo "Test 3: Test du minishell..."
echo "echo hello world" | timeout 3s ../minishell
