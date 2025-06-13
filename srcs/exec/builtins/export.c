/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:05:28 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/13 15:25:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	order(char **cpy);
int		assign_var(t_ast *node, int size);
int		create_var(t_ast *node, int size);
int		has_equal(const char *str);
int		replace_var_value(t_ast *node, int i);
int		var_exists(t_ast *node);

int	export_print(t_ast *node, char **cpy)
{
	int	i;

	order(cpy);
	i = -1;
	while (cpy[++i])
	{
		if (ft_strncmp(cpy[i], "_=/usr/bin/bash", 16))
		{
			ft_putstr_fd("declare -x ", node->cmd.fd_out);
			ft_putstr_fd(cpy[i], node->cmd.fd_out);
			ft_putchar_fd('\n', node->cmd.fd_out);
		}
	}
	clean_env_cpy(cpy, -1);
	return (set_exit_status(node, SUCCESS));
}

int	export_wo_args(t_ast *node, int size)
{
	char	**cpy;
	int		len;
	int		i;

	cpy = (char **) malloc((size + 1) * sizeof(char *));	// assez d'espace pour la nul-terminaison?
	if (!cpy)
		malloc_error(node);
	i = -1;
	while (node->root->envp[++i])
	{
		len = ft_strlen(node->root->envp[i]) + 1;
		cpy[i] = (char *) malloc(len * sizeof(char));
		if (!cpy[i])
		{
			clean_env_cpy(cpy, i);
			malloc_error(node);
		}
		ft_strlcat(cpy[i], node->root->envp[i], len);
	}
	cpy[i] = NULL;
	return (export_print(node, cpy));
}

// node->root->envp peut être vide si on unset manuellement chaque variable existante, mais le pointeur devrait tout de même pointer sur une variable
// partons du principe qu'on va pas gérer ça et voyons plus tard
int	export_bltn(t_ast *node)
{
	int	size;

	size = ft_char_tab_len(node->root->envp);
	if (!node->cmd.args[1])
		return (export_wo_args(node, size));
	else if (has_equal(node->cmd.args[1]))
		return (assign_var(node, size));
	else if (!var_exists(node))
		return (create_var(node, size));
	return (set_exit_status(node, SUCCESS));
}
