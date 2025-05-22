/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:54:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/21 14:58:12 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <limits.h>			// autorisé ?
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>			// autorisé ?
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include <signal.h>
# include "../libft/libft.h"

# ifndef SUCCESS
#  define SUCCESS 0
# endif

# ifndef FAILURE
#  define FAILURE 1
# endif

/* Enums */
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND_IF,
	NODE_OR_IF,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_HEREDOC,
	NODE_SUBSHELL
}	t_node_type;

typedef enum e_token_type
{
	DEFAULT = 0,
	WORD = 1,
	ASSIGNMENT = 2,
	QUOTE = 3,
	SINGLE_QUOTE = 4,
	DOUBLE_QUOTE = 5,
	REDIR = 6,
	REDIR_IN = 7,
	REDIR_OUT = 8,
	REDIR_APPEND = 9,
	HEREDOC = 10,
	OPERATOR = 11,
	PIPE = 12,
	AND = 13,
	OR = 14,
	SEMICOLON = 15,
	PARENTHESIS = 16,
	PAREN_OPEN = 17,
	PAREN_CLOSE = 18,
	SPECIAL_CARACTER = 19,
	ENV_VAR = 20,
	EXIT_STATUS = 21,
	ESCAPE = 22,
	COMMENT = 23,
	T_NEWLINE = 24,
	T_EOF = 25,
	UNKNOWN = 26
}	t_token_type;

/* Structs */
typedef struct s_cmd
{
	char		**args;
	char		*path;
	int			fd_in;
	int			fd_out;
}	t_cmd;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*root;
	struct s_ast	**children;
	t_cmd			cmd;
	char			**paths;
	char			**envp;
	char			*file;
}	t_ast;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_shell_data
{
	char	**envp;
	char	**paths;
	int		exit_status;
}	t_shell_data;

extern t_shell_data	g_shell;

/* Pseudo parsing functions */
t_ast	*make_ast(int  mode);
char	**ft_split_path(const char *s, char c);
char	**get_paths(void);

/* Execution functions */
void	close_redirs(t_cmd cmd);
void	dup_fds(t_ast ast);
void	exec_cmd(t_ast *ast, t_cmd cmd);
void	exec_pipe_child(t_ast *child);
void	get_cmd_path(t_cmd *cmd, char **paths);
void	make_heredoc(t_ast *node, t_cmd *cmd);
void	reset_std_fds(void);
void	unlink_heredoc(t_ast *child);
int		check_redirs(t_ast *child, t_cmd cmd);
int		exec_ast(t_ast *ast);
int		exec_pipe(t_ast **children);
int		exec_subshell(t_ast *node);
int		make_redirs(t_ast *ast);

/* Utilities functions */
void	clean_exit(t_ast *root, int status);
void	cleanup(t_ast *ast);
void	print_cmd(int mode);
void	print_options(void);
void	dup2_error(void);
void	malloc_error(t_ast *ast);
void	print_tree(t_ast *ast);
void	set_root_node(t_ast *ast, t_ast *root);
char	*make_cwd(void);
int		fork_error(void);

/* Pipe functions */
void	close_pipes(int fd[2][2], int i, int count);
void	exec_pipe_and(t_ast *node);
void	exec_pipe_child(t_ast *child);
void	exec_pipe_cmd(t_ast *node);
void	exec_pipe_or(t_ast *node);
void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i);
int		*init_pids(int count);
int		count_nodes(t_ast **children);
int		make_fork(void);
int		make_pipe(int fd[2]);
int		pipe_error(int *pids, int fd[2][2], int i, int count);
int		waitpids(int *pids, int count);

/* Builtins functions */
char	*get_str(const char *name);
int		cd(t_ast *node);
int		echo(t_ast	*node);
int		env(t_ast *node);
int		exec_builtin(t_ast *node);
int		is_builtin(t_cmd cmd);
int		pwd(t_ast *node);
//int		unset(t_ast *node);

/* Parsing functions */
t_node_type	token_to_node_type(t_token_type type);

t_token		**extract_token(char **command, t_token_type type, t_token **token_list);
t_token		**tokenize_input(char *command);
t_token		*create_token(void);
t_token		*get_token_at_index(t_token **token_list, int index);
t_token		*handle_token_type(char **command, t_token_type type, t_token *new_token);
t_token		*tokenize_operator(char **command, t_token *token);
t_token		*tokenize_parenthesis(char **command, t_token *token);
t_token		*tokenize_quote(char **command, t_token *token);
t_token		*tokenize_redir(char **command, t_token *token);
t_token		*tokenize_special_caracter(char **command, t_token *token);
t_token		*tokenize_word(char **command, t_token *token);

t_ast		**extract_redirections(t_token **tokens, int start, int end, t_ast *root);
t_ast		*create_ast(t_token **token_list);
t_ast		*create_ast_node(t_node_type type, t_ast *root);
t_ast		*create_command_node(char **args, t_ast *root);
t_ast		*create_operator_node(t_node_type type, t_ast *left, t_ast *right, t_ast *root);
t_ast		*create_redirection_node(t_node_type type, char *file, t_ast *root);
t_ast		*create_subshell_node(t_ast *child, t_ast *root);
t_ast		*parse_command_line(t_token **tokens, int start, int end, t_ast *root);
t_ast		*parse_input(char *command);
t_ast		*parse_simple_command(t_token **tokens, int start, int end, t_ast *root);

char		**extract_args(t_token **tokens, int start, int end);
//	char		*ft_strdup(const char *s);

//	void		*ft_memset(void *s, int c, size_t n);
void		add_child_to_node(t_ast *parent, t_ast *child);
void		define_bonus_operator(char **command, t_token *token, int *len);
void		define_double_redirection(char **command, t_token *token, int *len);
void		define_special_caracter(char **command, t_token *token);
void		free_ast(t_ast *node);
void		free_string_array(char **array);
void		free_token(t_token *token);
void		free_token_list(t_token **token_list);
void		link_token(t_token *token, t_token **token_list);
void		print_ast(t_ast *node, int depth);
void		print_token_list(t_token **token_list);

int			check_token(t_token *cur, t_token **stack, int *top);
int			count_args(t_token **tokens, int start, int end);
int			count_redirections(t_token **tokens, int start, int end);
int			count_tokens(t_token **token_list);
int			find_lowest_precedence_op(t_token **tokens, int start, int end);
int			find_matching_parenthesis(t_token **tokens, int open_pos, int end);
int			find_word_len(char *content);
int			get_operator_precedence(t_token_type type);
int			is_command_char(char c);
int			is_in_subshell(t_token **token_list, int index);
int			is_operator(char c);
int			is_parenthesis(char c);
int			is_quote(char c);
int			is_redirection(char c);
int			is_redirection_token(t_token_type type);
int			is_space(char c);
int			is_special_caracter(char c);
int			matching_parentheses(t_token **tokens, int start, int end);
int			operator_syntax_is_valid(t_token **token_list);
int			paren_syntax_is_valid(t_token **token_list);
int			quote_syntax_is_valid(t_token **token_list);
int			redir_syntax_is_valid(t_token **token_list);
int			syntax_is_valid(t_token **token_list);

#endif
