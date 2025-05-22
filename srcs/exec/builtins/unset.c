/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:18:00 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/22 14:58:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"


/* Each variable or function specified by name shall be unset. 
 *
 * If neither -f nor -v is specified, name refers to a variable; if a
 * variable by that name does not exist, it is unspecified whether a
 * function by that name, if any, shall be unset.
 *
 * Unsetting a variable or function that was not previously set shall
 * not be considered an error and does not cause the shell to abort.
 * Note that:
 *
 * 		VARIABLE=
 *
 * is not equivalent to an unset of VARIABLE; in the example,
 * VARIABLE is set to "". */

int	unset(t_ast *node)
{
	char	**new_env;
	int		i;
	int		j;

	i = ft_char_tab_len(node->root->envp);
	if (i == -1)
		return (FAILURE);
	new_env = (char **) malloc(i * sizeof(char *));
	if (!new_env)
		malloc_error(node);
	i = -1;
	j = -1;
	while (node->root->envp[++i])
	{
		if (ft_strncmp(node->root->envp[i], node->cmd.args[1], ft_strlen(node->cmd.args[1]) + 1))
		{
			new_env[++j] = (char *) malloc((ft_strlen(node->root->envp[i]) + 1) * sizeof(char));
			if (!new_env[j])
			{
				clean_env_cpy(new_env, j);
				malloc_error(node);
			}
			ft_strlcat(new_env[j], node->root->envp[i], ft_strlen(node->root->envp[i]) + 1);
		}
	}
	new_env[++j] = NULL;
	clean_envp(node->root->envp);
	node->root->envp = new_env;
	return (SUCCESS);
}
