/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cpy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:57:09 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/12 16:27:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	clean_env_cpy(char **env_cpy, int i)
{
	if (i == -1)
	{
		i = 0;
		while (env_cpy[i])
			i++;
	}
	while (--i >= 0)
		free(env_cpy[i]);
	free(env_cpy);
}

int	count_digits(int lvl)
{
	int	dig;

	dig = 0;
	while (lvl > 0)
	{
		lvl /= 10;
		dig ++;
	}
	return (dig);
}

void	set_shlvl(char **env_cpy)
{
	char	*level;
	int		len;
	int		lvl;
	int		i;

	i = -1;
	while (env_cpy[++i])
	{
		if (!ft_strncmp(env_cpy[i], "SHLVL=", 6))
		{
			lvl = ft_atoi(env_cpy[i] + 6) + 1;
			free(env_cpy[i]);
			len = count_digits(lvl) + 8;
			env_cpy[i] = (char *) malloc(len * sizeof(char));
			if (!env_cpy[i])
				malloc_error(NULL);
			ft_strlcat(env_cpy[i], "SHLVL=", len);
			level = ft_itoa(lvl);
			ft_strlcat(env_cpy[i], level, len);
			free(level);
		}
	}
}

char	**create_envp(void)
{
	char	**envp;
	char	*cwd;
	int		len;

	envp = (char **) malloc(4 * sizeof(char *));
	if (!envp)
		malloc_error(NULL);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	envp[0] = (char *) malloc(len * sizeof(char));
	if (!envp[0])
		malloc_error(NULL);
	ft_strlcat(envp[0], "PWD=", len);
	ft_strlcat(envp[0], cwd, len);
	free(cwd);
	envp[1] = (char *) malloc(8 * sizeof(char));
	if (!envp[1])
	{
		free(envp[0]);
		free(envp);
		malloc_error(NULL);
	}
	ft_strlcat(envp[1], "SHLVL=1", 8);
	envp[2] = (char *) malloc(15 * sizeof(char));
	if (!envp[2])
	{
		free(envp[0]);
		free(envp[1]);
		free(envp);
		malloc_error(NULL);
	}
	ft_strlcat(envp[2], "_=/usr/bin/env", 15);
	envp[3] = NULL;
	return (envp);
}

char	**copy_env(char **envp)
{
	char	**env_cpy;
	int	i;

	if (!envp)
		return (NULL);
	else if (!*envp)
		return (create_envp());
	i = 0;
	while (envp[i])
		i++;
	env_cpy = (char **) malloc(++i * sizeof(char *));
	if (!env_cpy)
		malloc_error(NULL);
	i = -1;
	while (envp[++i])
	{
		env_cpy[i] = (char *) malloc((ft_strlen(envp[i]) + 1) * sizeof(char));
		if (!env_cpy[i])
		{
			clean_env_cpy(env_cpy, i);
			malloc_error(NULL);
		}
		ft_strlcat(env_cpy[i], envp[i], ft_strlen(envp[i]) + 1);
	}
	env_cpy[i] = NULL;
	set_shlvl(env_cpy);
	return (env_cpy);
}
