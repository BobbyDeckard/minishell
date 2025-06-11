/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:21:14 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/10 17:36:27 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H


/* Functions */
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
void	clean_env_cpy(char **env_cpy, int i);
void	clean_envp(char **envp);
void	clean_exit(t_ast *root, int status);
void	cleanup(t_ast *ast);
void	print_cmd(int mode);
void	print_options(void);
void	dup2_error(void);
void	malloc_error(t_ast *ast);
void	print_tree(t_ast *ast);
void	set_root_node(t_ast *ast, t_ast *root);
char	**copy_env(char **envp);
char	*make_cwd(void);
int		fork_error(void);
int		ft_char_tab_len(char **tab);

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
int		export_bltn(t_ast *node);
int		is_builtin(t_cmd cmd);
int		pwd(t_ast *node);
int		unset(t_ast *node);

#endif
