/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:50:38 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/27 14:51:19 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	clean_envp(char **envp)
{
	int	i;

	i = -1;
	while (envp && envp[++i])
		free(envp[i]);
	free(envp);
}

void	clean_paths(char **paths)
{
	int	i;

	i = -1;
	while (paths && paths[++i])
	{
		free(paths[i]);
	}
	free(paths);
}

void	clean_ast(t_ast *ast)
{
	int	i;

	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			clean_ast(ast->children[i]);
	}
	if (ast->cmd.args)
	{
		i = -1;
		while (ast->cmd.args[++i])
			free(ast->cmd.args[i]);
		free(ast->cmd.args);
	}
	free(ast);
}

//	cleans up the pointers allocated for the AST and for the paths
void	cleanup(t_ast *root)
{
	clean_paths(root->paths);
//	clean_envp(root->envp);
	clean_ast(root);
}

void	clean_exit(t_ast *root, int status)
{
	cleanup(root);
	exit(status);
}
