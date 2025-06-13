/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:28:59 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/13 15:25:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	ft_char_tab_len(char **tab)
{
	int	i;

	if (!tab)
		return (-1);
	i = 0;
	while (tab[i])
		i++;
	return (i);
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

char	*get_var_name(t_ast *node)
{
	char	*name;
	int		i;

	i = 0;
	while (node->cmd.args[1][i] != '=')
		i++;
	name = (char *) malloc(++i * sizeof(char));
	if (!name)
		malloc_error(node);
	while (--i >= 0)
		name[i] = node->cmd.args[1][i];
	return (name);
}

int	var_exists(t_ast *node)
{
	char	*name;
	int		i;

	name = get_var_name(node);
	i = -1;
	while (node->root->envp[++i])
	{
		if (!ft_strncmp(name, node->root->envp[i], ft_strlen(name)))
			return (1);
	}
	return (0);
}

void	order(char **cpy)
{
	char	*ptr;
	int		i;
	int		j;

	i = -1;
	while (cpy[++i])
	{
		j = i;
		while (cpy[++j])
		{
			if (ft_strncmp(cpy[i], cpy[j], ft_strlen(cpy[i]) + 1) > 0)
			{
				ptr = cpy[i];
				cpy[i] = cpy[j];
				cpy[j] = ptr;
			}
		}
	}
}

