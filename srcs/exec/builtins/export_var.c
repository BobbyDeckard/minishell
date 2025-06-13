/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:13:23 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/13 15:13:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	**make_new_env(t_ast *node, int size);
char	*get_var_name(t_ast *node);

int	replace_var_value(t_ast *node, int i)
{
	int	len;

	free(node->root->envp[i]);
	len = ft_strlen(node->cmd.args[1]) + 1;
	node->root->envp[i] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[i])
		malloc_error(node);
	ft_strlcat(node->root->envp[i], node->cmd.args[1], len);
	return (set_exit_status(node, SUCCESS));
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
	return (set_exit_status(node, SUCCESS));
}

int	assign_var(t_ast *node, int size)
{
	char	*name;
	int		i;

	name = get_var_name(node);
	i = -1;
	while (node->root->envp[++i])
	{
		if (!ft_strncmp(name, node->root->envp[i], ft_strlen(name)))
			break ;
	}
	if (node->root->envp[i])
		return (replace_var_value(node, i));
	else
		return (create_var(node, size));
}
