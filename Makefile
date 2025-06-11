# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pitran <pitran@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/06 11:50:43 by pitran            #+#    #+#              #
#    Updated: 2025/06/11 11:58:33 by pitran           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LFLAGS = -lreadline -lncurses

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
              srcs/$(PARSING_DIR)/parse.c

SRC_SYNTAX = srcs/$(SYNTAX_DIR)/syntax_check.c \
             srcs/$(SYNTAX_DIR)/other_type_check.c \
             srcs/$(SYNTAX_DIR)/parenthesis_check.c

SRC_AST = srcs/$(AST_DIR)/ast_builder.c \
          srcs/$(AST_DIR)/parsing.c \
          srcs/$(AST_DIR)/nodes.c \
          srcs/$(AST_DIR)/simple_command.c \
          srcs/$(AST_DIR)/utils.c

SRC_SIGNALS = srcs/$(SIGNALS_DIR)/signals_setup.c \
              srcs/$(SIGNALS_DIR)/signals_handler.c

SRC_DEBUG = srcs/debug/ast_utils.c \
            srcs/debug/debug.c \
            srcs/debug/print.c \
            srcs/debug/presets.c

SRC_EXEC = srcs/$(EXEC_DIR)/exec.c \
           srcs/$(EXEC_DIR)/redirs.c \
           srcs/$(EXEC_DIR)/heredoc.c \
           srcs/$(EXEC_DIR)/exec_utils.c \
           srcs/$(EXEC_DIR)/get_path.c \
           srcs/$(EXEC_DIR)/pipe.c \
           srcs/$(EXEC_DIR)/pipe_utils.c \
           srcs/$(EXEC_DIR)/pipe_utils2.c \
           srcs/$(EXEC_DIR)/redirs_utils.c \
           srcs/$(EXEC_DIR)/subshell.c

SRC_BUILTINS = srcs/$(BUILTINS_DIR)/builtins.c \
               srcs/$(BUILTINS_DIR)/pwd.c \
               srcs/$(BUILTINS_DIR)/cd.c \
               srcs/$(BUILTINS_DIR)/echo.c \
               srcs/$(BUILTINS_DIR)/env.c \
				srcs/$(BUILTINS_DIR)/env_utils.c \
               srcs/$(BUILTINS_DIR)/export.c \
               srcs/$(BUILTINS_DIR)/unset.c

SRC_UTILS = srcs/$(UTILS_DIR)/cwd.c \
            srcs/$(UTILS_DIR)/env_cpy.c \
            srcs/$(UTILS_DIR)/errors.c \
            srcs/$(UTILS_DIR)/ft_split_path.c \
            srcs/$(UTILS_DIR)/general_utils.c \
            srcs/$(UTILS_DIR)/cleanup.c

# ========== COMBINE ALL SOURCES ==========
SRCS = $(SRC_MAIN) $(SRC_PARSING) $(SRC_SYNTAX) $(SRC_AST) $(SRC_SIGNALS) $(SRC_DEBUG) $(SRC_EXEC) $(SRC_BUILTINS) $(SRC_UTILS)

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

fclean: clean
	@echo "\033[31m🗑️  Removing $(NAME) and libft.a...\033[0m"
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

# ========== DEBUGGING TOOLS ==========
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

# ========== NORMINETTE ==========
norm:
	@echo "\033[35m📏 Checking norminette...\033[0m"
	@norminette $(SRCS) incl/ | grep -v "OK" || echo "\033[32m✅ All files are norm compliant!\033[0m"

# ========== DEBUG INFO ==========
debug:
	@echo "\033[33m🐛 Debug info:\033[0m"
	@echo "SRCS count: $(words $(SRCS))"
	@echo "OBJS count: $(words $(OBJS))"
	@echo "LIBFT: $(LIBFT)"
	@echo "First 5 sources:"
	@echo "$(wordlist 1,5,$(SRCS))"

# ========== HELP ==========
help:
	@echo "\033[34m📖 Available targets:\033[0m"
	@echo "  all        - Compile libft and minishell"
	@echo "  clean      - Remove object files"
	@echo "  fclean     - Remove object files and executables"
	@echo "  re         - Full recompilation"
	@echo "  test       - Run minishell"
	@echo "  test_signals - Test signal handling"
	@echo "  valgrind   - Run with valgrind"
	@echo "  norm       - Check norminette"
	@echo "  debug      - Show debug info"
	@echo "  help       - Show this help"

.PHONY: all clean fclean re valgrind test test_signals norm debug help