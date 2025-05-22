/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:15:47 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/22 17:37:29 by imeulema         ###   ########.fr       */
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
	env_cpy[0] = node->cmd.args[1];
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

void	order(char **ordered)
{
	char	*ptr;
	int		i;
	int		j;

	i = -1;
	while (ordered[++i])
	{
		j = i;
		while (ordered[++j])
		{
			if (ft_strncmp(ordered[i], ordered[j], ft_strlen(ordered[i]) + 1) > 0)
			{
				ptr = ordered[i];
				ordered[i] = ordered[j];
				ordered[j] = ptr;
			}
		}	
	}
}

void	print_export(t_ast *node, char **ordered)
{
	int	i;

	i = -1;
	while (ordered[++i])
	{
		ft_putstr_fd("declare -x ", node->cmd.fd_out);
		ft_putstr_fd(ordered[i], node->cmd.fd_out);
		ft_putchar_fd('\n', node->cmd.fd_out);
	}
}

int	export_print(t_ast *node, int size)
{
	char	**ordered;
	int		i;

	ordered = (char **) malloc((size + 1) * sizeof(char *));
	if (!ordered)
		malloc_error(node);
	i = -1;
	while (node->root->envp[++i])
	{
		ordered[i] = (char *) malloc((ft_strlen(node->root->envp[i]) + 1) * sizeof(char));
		if (!ordered[i])
		{
			clean_env_cpy(ordered, i);
			malloc_error(node);
		}
		ft_strlcat(ordered[i], node->root->envp[i], ft_strlen(node->root->envp[i]) + 1);
	}
	ordered[i] = NULL;
	order(ordered);
	print_export(node, ordered);
	clean_env_cpy(ordered, -1);
	return (SUCCESS);
}

int	has_equal(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '=')
			return (1);
	}
	return (0);
}

int	assign_var(t_ast *node, int size)
{
	if (size)
		return (FAILURE);
	else if (node)
		return (FAILURE);
	else
		return (FAILURE);
}

int	create_var(t_ast *node, int size)
{
	int	len;
	
	len = ft_strlen(node->cmd.args[1]) + 1;
	node->root->envp = make_new_env(node, size + 1);
	node->root->envp[size] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[size])
		malloc_error(node);
	ft_strlcat(node->root->envp[size], node->cmd.args[1], len);
	node->root->envp[++size] = NULL;
	return (SUCCESS);
}

int	export_bltn(t_ast *node)
{
	int		size;

	size = ft_char_tab_len(node->root->envp);
	if (node->cmd.args[1] && size == -1)
		return (create_env_cpy(node));
	else if (size == -1)
		return (FAILURE);					// not sure this is the behaviour of export
	else if (!node->cmd.args[1])
		return (export_print(node, size));
	else if (has_equal(node->cmd.args[1]))
		return (assign_var(node, size));
	else
		return (create_var(node, size));
	node->root->envp = make_new_env(node, size + 1);
	node->root->envp[size] = (char *) malloc((ft_strlen(node->cmd.args[1]) + 1) * sizeof(char));
	if (!node->root->envp[size])
		malloc_error(node);
	ft_strlcat(node->root->envp[size], node->cmd.args[1], ft_strlen(node->cmd.args[1]) + 1);
	node->root->envp[size + 1] = NULL;
	return (SUCCESS);
}
