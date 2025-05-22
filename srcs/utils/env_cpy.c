/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:57:09 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/22 11:30:28 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	clean_env_cpy(char **env_cpy, int i)
{
	while (--i >= 0)
		free(env_cpy[i]);
	free(env_cpy);
}

char	**init_env_cpy(char **envp)
{
	char	**env_cpy;
	int	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
		i++;
	env_cpy = (char **) malloc(i * sizeof(char *));
	if (!env_cpy)
		malloc_error(NULL);
	printf("\nAllocated %d pointers in env_cpy, ready to copy\n", i);
	i = -1;
	while (envp[++i])
	{
		env_cpy[i] = (char *) malloc(ft_strlen(envp[i]) * sizeof(char));
		if (!env_cpy[i])
		{
			clean_env_cpy(env_cpy, i);
			malloc_error(NULL);
		}
		ft_strlcat(env_cpy[i], envp[i], ft_strlen(envp[i]));
		printf("Copied entry no. %d: %s\n", i, env_cpy[i]);
	}
	printf("\n");
	env_cpy[i] = NULL;
	return (env_cpy);
}
