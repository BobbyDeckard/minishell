/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:57:25 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/27 14:51:57 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static unsigned int	get_len(const char *s, unsigned int i, char c)
{
	unsigned int	len;

	len = 2;
	while (s[i] && s[i] != c)
	{
		i++;
		len++;
	}
	return (len);
}

static unsigned int	count_elements(const char *s, char c)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 1;
	while (s[i] == c && c)
		i++;
	while (s[i])
	{
		count++;
		while (s[i] && s[i] != c)
			i++;
		while (s[i] == c && c)
			i++;
	}
	return (count);
}

static void	free_tab(char **tab)
{
	unsigned int	index;

	index = -1;
	while (tab[++index])
		free(tab[index]);
	free(tab);
}

static char	**make_splits(char **tab, const char *s, unsigned int i, char c)
{
	unsigned int	index;
	unsigned int	j;

	index = 0;
	while (s[i])
	{
		j = 0;
		tab[index] = (char *) malloc(get_len(s, i, c) * sizeof(char));
		if (!tab[index])
		{
			free_tab(tab);
			return (0);
		}
		while (s[i] && s[i] != c)
			tab[index][j++] = s[i++];
		tab[index][j++] = '/';
		tab[index][j] = 0;
		while (s[i] == c && c)
			i++;
		index++;
	}
	tab[index] = NULL;
	return (tab);
}

// variation de ft_split, qui alloue un char de plus et ajoute un / final
char	**ft_split_path(const char *s, char c)
{
	char			**tab;
	unsigned int	i;

	tab = (char **) malloc(count_elements(s, c) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	tab[0] = 0;
	while (s[i] == c && c)
		i++;
	return (make_splits(tab, s, i, c));
}
