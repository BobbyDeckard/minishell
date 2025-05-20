/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:27:55 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/23 14:21:51 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	get_len(const char *s, unsigned int i, char c)
{
	unsigned int	len;

	len = 1;
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

void	free_tab(char **tab)
{
	unsigned int	index;

	index = -1;
	while (tab[++index])
		free(tab[index]);
	free(tab);
}

char	**make_splits(char **tab, const char *s, unsigned int i, char c)
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
		tab[index][j] = 0;
		while (s[i] == c && c)
			i++;
		index++;
	}
	tab[index] = NULL;
	return (tab);
}

char	**ft_split(const char *s, char c)
{
	char			**tab;
	unsigned int	i;

	if (!s)
		return (NULL);
	tab = (char **) malloc(count_elements(s, c) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	tab[0] = 0;
	while (s[i] == c && c)
		i++;
	return (make_splits(tab, s, i, c));
}
