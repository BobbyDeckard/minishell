/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:15:47 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/22 15:01:07 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/* The shell shall give the export attribute to the variables
 * corresponding to the specified names, which shall cause them to be
 * in the environment of subsequently executed commands. If the name
 * of a variable is followed by =word, then the value of that
 * variable shall be set to word. */

int	create_env_cpy(t_ast *node)
{
	char	**env_cpy;

	env_cpy = (char **) malloc(2 * sizeof(char *));
	if (!env_cpy)
		malloc_error(node);
	env_cpy[0] = (char *) malloc((ft_strlen(node->cmd.args[1]) + 1) * sizeof(char));
	if (!env_cpy[0])
	{
		free(env_cpy);
		malloc_error(node);
	}
	env_cpy[1] = NULL;
	node->root->envp = env_cpy;
	return (SUCCESS);
}

char	**make_new_env(t_ast *node, int size)
{
	char	**new_env;
	int		i;

	new_env = (char **) malloc(size * sizeof(char *));
	if (!new_env)
		malloc_error(node);
	i = -1;
	while (node->root->envp[++i])
	{
		new_env[i] = (char *) malloc((ft_strlen(node->root->envp[i]) + 1) * sizeof(char));
		if (!new_env[i])
		{
			clean_env_cpy(new_env, i);
			malloc_error(NULL);
		}
		ft_strlcat(new_env[i], node->root->envp[i], ft_strlen(node->root->envp[i]) + 1);
		free(node->root->envp[i]);
	}
	new_env[i] = NULL;
	free(node->root->envp);
	return (new_env);
}

int	export_bltn(t_ast *node)
{
	int		size;

	size = ft_char_tab_len(node->root->envp);
	if (size == -1)
		return (create_env_cpy(node));
	node->root->envp = make_new_env(node, size + 1);
	node->root->envp[size] = (char *) malloc((ft_strlen(node->cmd.args[1]) + 1) * sizeof(char));
	if (!node->root->envp[size])
		malloc_error(node);
	ft_strlcat(node->root->envp[size], node->cmd.args[1], ft_strlen(node->cmd.args[1]) + 1);
	node->root->envp[size + 1] = NULL;
	return (SUCCESS);
}
