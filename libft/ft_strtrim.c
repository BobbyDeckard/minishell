/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:09:48 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 17:39:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_set(const char c, const char *set)
{
	unsigned int	i;

	i = -1;
	while (set[++i])
		if (set[i] == c)
			return (1);
	return (0);
}

static unsigned int	count_front_trim(const char *s1, const char *set)
{
	unsigned int	i;

	i = 0;
	while (check_set(s1[i], set))
		i++;
	return (i);
}

static unsigned int	count_back_trim(const char *s1, const char *set)
{
	unsigned int	i;
	unsigned int	count;

	count = 0;
	i = ft_strlen(s1);
	while (--i > 0 && check_set(s1[i], set))
		count++;
	return (count);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char			*trim;
	unsigned int	i;
	unsigned int	j;
	unsigned int	front_trim;
	unsigned int	back_trim;

	if (!s1 || !set)
		return (NULL);
	back_trim = 0;
	front_trim = count_front_trim(s1, set);
	if (front_trim != ft_strlen(s1))
		back_trim = count_back_trim(s1, set);
	trim = (char *) malloc((ft_strlen(s1) - front_trim - back_trim + 1)
			* sizeof(char));
	if (!trim)
		return (NULL);
	i = front_trim - 1;
	j = -1;
	while (++i + back_trim < ft_strlen(s1))
		trim[++j] = s1[i];
	trim[++j] = 0;
	return (trim);
}
