# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/27 14:35:02 by imeulema          #+#    #+#              #
#    Updated: 2025/05/22 12:02:57 by imeulema         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wextra -Wall -Werror -g 
LFLAGS = -lreadline
RM = rm
RMFLAGS = -f
MAKE = make
UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
	LFLAGS += -L /opt/homebrew/opt/readline/lib -I /opt/homebrew/opt/readline/include/readline
endif

# SOURCES
SRCS = $(addprefix $(PATH_SRCS), $(SRCS_FILES))
SRCS_FILES = main.c \
			 $(EXEC) \
			 $(UTILS) \
			 $(PARSING) \
			 $(DEBUG)
PATH_SRCS = srcs/
PATH_OBJS = objs/
OBJS = $(patsubst $(PATH_SRCS)%.c, $(PATH_OBJS)%.o, $(SRCS))

# SOURCE FILES
# PSEUDO = $(addprefix pseudo_parsing/, $(PSEUDO_FILES))
# PSEUDO_FILES = presets.c
EXEC = $(addprefix exec/, $(EXEC_FILES))
EXEC_FILES = exec.c \
			 exec_utils.c \
			 get_path.c \
			 heredoc.c \
			 pipe.c \
			 pipe_utils.c \
			 pipe_utils2.c \
			 redirs.c \
			 redirs_utils.c \
			 subshell.c \
			 $(BUILTINS)
BUILTINS = $(addprefix builtins/, $(BLTIN_FILES))
BLTIN_FILES = builtins.c \
			  cd.c \
			  echo.c \
			  env.c \
			  env_utils.c \
			  export.c \
			  pwd.c \
			  unset.c
UTILS = $(addprefix utils/, $(UTILS_FILES))
UTILS_FILES = cleanup.c \
			  cwd.c \
			  env_cpy.c \
			  errors.c \
			  ft_split_path.c \
			  general_utils.c
PARSING = $(addprefix parsing/, $(PARS_FILES))
PARS_FILES = caracter_type.c \
			 caracter_type2.c \
			 helpers.c \
			 parse.c \
			 retrieve.c \
			 tokenize.c \
			 tokenize2.c \
			 tokens.c \
			 $(AST) \
			 $(SYNTAX)
AST = $(addprefix ast/, $(AST_FILES))
AST_FILES = ast_builder.c \
			nodes.c \
			parsing.c \
			simple_command.c \
			utils.c
SYNTAX = $(addprefix syntax_check/, $(SYNT_FILES))
SYNT_FILES = other_type_check.c \
			 parenthesis_check.c \
			 syntax_check.c
DEBUG = $(addprefix debug/, $(DBG_FILES))
DBG_FILES = ast_utils.c \
			debug.c \
			presets.c \
			print.c

# INCLUDES
HEADERS = incl/minishell.h
LIBFT = libft/libft.a

# TERMINAL COLORS
RED = \033[0;31m
YELLOW = \033[0;33m
GREEN = \033[0;32m
RESET = \033[0m

# RULES
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	echo "$(YELLOW)compiling $(NAME)$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(LIBFT) -o $(NAME)
	echo "$(GREEN)$(NAME) compiled!$(RESET)"

$(PATH_OBJS)%.o: $(PATH_SRCS)%.c $(HEADERS) $(LIBFT)
	echo "$(YELLOW)compiling $<$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	echo "$(YELLOW)compiling libft$(RESET)"
	$(MAKE) -C libft --no-print-directory

clean:
	$(RM) $(RMFLAGS) $(OBJS)
	$(MAKE) clean -C libft --no-print-directory

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)
	$(MAKE) fclean -C libft --no-print-directory
	echo "$(GREEN)Full clean done.$(RESET)"

re: fclean all

.PHONY: all clean fclean re

.SILENT:
