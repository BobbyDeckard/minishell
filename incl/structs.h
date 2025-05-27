/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:44:46 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/27 14:46:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

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

#endif
