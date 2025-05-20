/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:54:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 20:11:00 by imeulema         ###   ########.fr       */
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
int		cd(t_ast *cd);
int		echo(t_ast	*echo);
int		env(t_ast *env);
int		exec_builtin(t_ast *cmd);
int		is_builtin(t_cmd cmd);
int		pwd(t_ast *pwd);

#include <string.h>

t_ast	*parse_command(char **tokens, int *i);
t_ast	*parse_pipe(char **tokens, int *i);
t_ast	*parse_and_or(char **tokens, int *i);
t_ast	*make_ast_from_input(const char *input);

#endif
