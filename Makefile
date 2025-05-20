# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/27 14:35:02 by imeulema          #+#    #+#              #
#    Updated: 2025/05/13 19:42:01 by imeulema         ###   ########.fr        #
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
			 $(BUILTINS)
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
			 subshell.c
UTILS = $(addprefix utils/, $(UTILS_FILES))
UTILS_FILES = ast_utils.c \
			  cleanup.c \
			  cwd.c \
			  errors.c \
			  ft_split_path.c \
			  print.c
BUILTINS = $(addprefix builtins/, $(BLTIN_FILES))
BLTIN_FILES = builtins.c \
			  cd.c \
			  env.c \
			  env_utils.c \
			  echo.c \
			  pwd.c

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
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) $(LIBFT) -o $(NAME)
	echo "$(GREEN)$(NAME) compiled!$(RESET)"

$(PATH_OBJS)%.o: $(PATH_SRCS)%.c $(HEADERS) $(LIBFT)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
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

# .SILENT:
