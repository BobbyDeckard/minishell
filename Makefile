# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pitran <pitran@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/06 11:50:43 by pitran            #+#    #+#              #
#    Updated: 2025/06/13 16:20:43 by pitran           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -O0
LFLAGS = -lreadline -lncurses
UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	LFLAGS += -L /opt/homebrew/opt/readline/lib -I /opt/homebrew/opt/readline/include/readline
endif

# ========== LIBFT ==========
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# ========== SOURCE DIRECTORIES ==========
PARSING_DIR = parsing
SYNTAX_DIR = $(PARSING_DIR)/syntax_check
AST_DIR = $(PARSING_DIR)/ast
SIGNALS_DIR = signals
EXEC_DIR = exec
BUILTINS_DIR = $(EXEC_DIR)/builtins
UTILS_DIR = $(EXEC_DIR)/utils

# ========== SOURCE FILES ==========
SRC_MAIN = srcs/main.c

SRC_PARSING = srcs/$(PARSING_DIR)/retrieve.c \
              srcs/$(PARSING_DIR)/helpers.c \
              srcs/$(PARSING_DIR)/caracter_type.c \
              srcs/$(PARSING_DIR)/caracter_type2.c \
              srcs/$(PARSING_DIR)/tokenize.c \
              srcs/$(PARSING_DIR)/tokenize2.c \
              srcs/$(PARSING_DIR)/tokens.c \
              srcs/$(PARSING_DIR)/parse.c \
              srcs/$(PARSING_DIR)/expansion_detection.c

SRC_SYNTAX = srcs/$(SYNTAX_DIR)/syntax_check.c \
             srcs/$(SYNTAX_DIR)/operator_check.c \
             srcs/$(SYNTAX_DIR)/parenthesis_check.c \
			 srcs/$(SYNTAX_DIR)/quote_check.c \
			 srcs/$(SYNTAX_DIR)/redir_check.c \
			 

SRC_AST = srcs/$(AST_DIR)/ast_builder.c \
          srcs/$(AST_DIR)/parsing.c \
          srcs/$(AST_DIR)/nodes.c \
          srcs/$(AST_DIR)/simple_command.c \
          srcs/$(AST_DIR)/utils.c

SRC_SIGNALS = srcs/$(SIGNALS_DIR)/signals_setup.c \
              srcs/$(SIGNALS_DIR)/signals_handler.c

SRC_DEBUG = srcs/debug/ast_utils.c

SRC_EXEC = srcs/$(EXEC_DIR)/exec.c \
           srcs/$(EXEC_DIR)/exec_utils.c \
           srcs/$(EXEC_DIR)/get_path.c \
           srcs/$(EXEC_DIR)/heredoc.c \
           srcs/$(EXEC_DIR)/pipe.c \
           srcs/$(EXEC_DIR)/pipe_utils.c \
           srcs/$(EXEC_DIR)/pipe_utils2.c \
           srcs/$(EXEC_DIR)/redirs.c \
           srcs/$(EXEC_DIR)/redirs_utils.c \

SRC_BUILTINS = srcs/$(BUILTINS_DIR)/builtins.c \
               srcs/$(BUILTINS_DIR)/cd.c \
               srcs/$(BUILTINS_DIR)/echo.c \
               srcs/$(BUILTINS_DIR)/env.c \
               srcs/$(BUILTINS_DIR)/env_utils.c \
               srcs/$(BUILTINS_DIR)/exit.c \
               srcs/$(BUILTINS_DIR)/export.c \
               srcs/$(BUILTINS_DIR)/pwd.c \
               srcs/$(BUILTINS_DIR)/unset.c

SRC_UTILS = srcs/$(UTILS_DIR)/cleanup.c \
			srcs/$(UTILS_DIR)/cwd.c \
            srcs/$(UTILS_DIR)/env_cpy.c \
            srcs/$(UTILS_DIR)/errors.c \
            srcs/$(UTILS_DIR)/ft_split_path.c \
            srcs/$(UTILS_DIR)/general_utils.c

# ========== COMBINE ALL SOURCES ==========
SRCS = $(SRC_MAIN) $(SRC_PARSING) $(SRC_SYNTAX) $(SRC_AST) $(SRC_SIGNALS) $(SRC_EXEC) $(SRC_BUILTINS) $(SRC_UTILS)

# ========== OBJECT FILES ==========
OBJS = $(SRCS:.c=.o)

# ========== INCLUDE DIRECTORIES ==========
INCLUDES = -I./incl -I./$(LIBFT_DIR)

# ========== MAIN RULES ==========
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) $(LFLAGS) -o $(NAME)
	@echo "\033[32m✅ $(NAME) compiled successfully!\033[0m"

$(LIBFT):
	@echo "\033[33m🔨 Compiling libft...\033[0m"
	@make -C $(LIBFT_DIR)

%.o: %.c
	@echo "\033[36m🔧 Compiling $<...\033[0m"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ========== CLEANING RULES ==========
clean:
	@echo "\033[31m🗑️  Cleaning object files...\033[0m"
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean clean_tests
	@echo "\033[31m🗑️  Removing $(NAME) and libft.a...\033[0m"
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

# ========== VARIABLE DETECTION TESTING ==========
test_detection: $(LIBFT)
	@echo "\033[32m🧪 Testing variable detection...\033[0m"
	@echo "Compiling test..."
	@$(CC) $(CFLAGS) $(INCLUDES) test_detection_only.c $(SRC_PARSING) $(SRC_SYNTAX) $(SRC_AST) $(LIBFT) -o test_detection
	@echo "Running detection tests..."
	@./test_detection
	@echo "\033[32m✅ Detection tests completed!\033[0m"

# ========== DEBUGGING TOOLS ==========
debug: $(LIBFT)
	@echo "\033[33m🐛 Compiling debug version...\033[0m"
	$(CC) $(CFLAGS) -DDEBUG $(INCLUDES) $(SRCS) $(LIBFT) $(LFLAGS) -o $(NAME)_debug
	@echo "\033[32m✅ Debug version ready: ./$(NAME)_debug\033[0m"

valgrind: $(NAME)
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			./$(NAME)

# ========== TESTING ==========
test: $(NAME)
	@echo "\033[32m🧪 Testing minishell...\033[0m"
	@echo "Type 'exit' to quit the test"
	./$(NAME)

test_signals: $(NAME)
	@echo "\033[32m🧪 Testing signals...\033[0m"
	@echo "Test Ctrl-C (interrupt), Ctrl-\\ (quit), Ctrl-D (EOF)"
	@echo "Try: sleep 10 then press Ctrl-C or Ctrl-\\"
	./$(NAME)

test_variables: $(NAME)
	@echo "\033[32m🧪 Testing variable expansion...\033[0m"
	@echo "Test commands with variables:"
	@echo "  echo \$$USER"
	@echo "  echo \$$?"
	@echo "  echo hello\$$HOME"
	./$(NAME)

# ========== EXPANSION TESTING (pour votre binôme) ==========
test_expansion_ready: test_detection
	@echo "\033[33m📋 Variables detection is ready for expansion!\033[0m"
	@echo "Your teammate can now implement expansion using:"
	@echo "  - tokens with needs_expansion = 1"
	@echo "  - ENV_VAR tokens (content = variable name)"
	@echo "  - EXIT_STATUS tokens (content = \"?\")"
	@echo "  - WORD tokens with embedded variables"

# ========== NORMINETTE ==========
norm:
	@echo "\033[35m📏 Checking norminette...\033[0m"
	@norminette $(SRCS) incl/ | grep -v "OK" || echo "\033[32m✅ All files are norm compliant!\033[0m"

norm_parsing:
	@echo "\033[35m📏 Checking norminette for parsing files...\033[0m"
	@norminette $(SRC_PARSING) $(SRC_SYNTAX) $(SRC_AST) | grep -v "OK" || echo "\033[32m✅ Parsing files are norm compliant!\033[0m"

# ========== CLEANUP TESTS ==========
clean_tests:
	@echo "\033[31m🗑️  Cleaning test files...\033[0m"
	@rm -f test_detection
	@rm -f $(NAME)_debug
	@rm -f valgrind-out.txt

# ========== DEBUG INFO ==========
debug_info:
	@echo "\033[33m🐛 Debug info:\033[0m"
	@echo "SRCS count: $(words $(SRCS))"
	@echo "OBJS count: $(words $(OBJS))"
	@echo "LIBFT: $(LIBFT)"
	@echo "New files added:"
	@echo "  - srcs/parsing/expansion_detection.c"
	@echo "Removed debug files for clean submission"

# ========== PROJECT STATUS ==========
status:
	@echo "\033[34m📊 Project Status:\033[0m"
	@echo "✅ Parsing: $(words $(SRC_PARSING)) files"
	@echo "✅ Syntax Check: $(words $(SRC_SYNTAX)) files"
	@echo "✅ AST: $(words $(SRC_AST)) files"
	@echo "✅ Execution: $(words $(SRC_EXEC)) files"
	@echo "✅ Builtins: $(words $(SRC_BUILTINS)) files"
	@echo "✅ Utils: $(words $(SRC_UTILS)) files"
	@echo "✅ Signals: $(words $(SRC_SIGNALS)) files"
	@echo "📁 Total: $(words $(SRCS)) source files"
	@echo ""
	@echo "🎯 Ready for:"
	@echo "  - Variable detection testing (make test_detection)"
	@echo "  - Full minishell testing (make test)"
	@echo "  - Norminette check (make norm)"

# ========== HELP ==========
help:
	@echo "\033[34m📖 Available targets:\033[0m"
	@echo ""
	@echo "\033[32mBuild targets:\033[0m"
	@echo "  all              - Compile libft and minishell"
	@echo "  clean            - Remove object files"
	@echo "  fclean           - Remove object files and executables"
	@echo "  re               - Full recompilation"
	@echo ""
	@echo "\033[32mTesting targets:\033[0m"
	@echo "  test             - Run minishell interactively"
	@echo "  test_detection   - Test variable detection specifically"
	@echo "  test_signals     - Test signal handling"
	@echo "  test_variables   - Test variable expansion"
	@echo "  test_expansion_ready - Check if ready for expansion implementation"
	@echo ""
	@echo "\033[32mDebugging targets:\033[0m"
	@echo "  debug            - Compile with debug flags"
	@echo "  valgrind         - Run with valgrind"
	@echo "  debug_info       - Show compilation info"
	@echo ""
	@echo "\033[32mQuality targets:\033[0m"
	@echo "  norm             - Check norminette on all files"
	@echo "  norm_parsing     - Check norminette on parsing files only"
	@echo "  status           - Show project status"
	@echo ""
	@echo "\033[32mCleanup targets:\033[0m"
	@echo "  clean_tests      - Remove test executables"

.PHONY: all clean fclean re test test_detection test_signals test_variables test_expansion_ready valgrind norm norm_parsing debug debug_info status clean_tests help
