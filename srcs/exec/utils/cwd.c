/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 23:06:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/10 17:00:27 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*trunc_cwd(char *full)
{
	char	*cwd;
	char	*ptr;
	int		len;
	int		i;
	int		j;

	ptr = full;
	while (*ptr)
		ptr++;
	while (*ptr != '/')
		ptr--;
	ptr++;
	len = ft_strlen(ptr);
	cwd = (char *) malloc(256 * sizeof(char));
	if (!cwd)
	{
		exit(1);
	}
	i = 0;
	while (i < len + 19)
		cwd[i++] = 0;
	ft_strlcat(cwd, "\e[1;36m", 256);
	i = 0;
	j = ft_strlen(cwd);
	while (i < len)
	{
		cwd[j++] = ptr[i++];
	}
	ft_strlcat(cwd, "\x1b[0m", 256);
//	j = ft_strlen(cwd);
//	cwd[j] = ' ';
//	ft_strlcat(cwd, "\x1b[0m", 256);
//	j = ft_strlen(cwd);
//	cwd[j] = '>';
//	ft_strlcat(cwd, "\x1b[0m", 256);
	j = ft_strlen(cwd);
	cwd[j] = ' ';
	free(full);
	return (cwd);
}

// permet d'afficher le directoire de travail
// actuel (ou cwd pour Current Working Directory), récupère le path
// complet avec getcwd(), puis coupe tout ce qui précède le dernier /
// (compris) et ajoute " > "
char	*make_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	cwd = trunc_cwd(cwd);
	return (cwd);
}