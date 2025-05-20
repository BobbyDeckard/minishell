/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:57:40 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 17:41:08 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	get_size(const char *s, unsigned int start, size_t len)
{
	unsigned int	count;

	count = 1;
	while (s[start++])
		count++;
	if (count > len)
		return (len + 1);
	return (count);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	else if (start >= ft_strlen(s))
		return ((char *) ft_calloc(1, sizeof(char)));
	sub = (char *) malloc(get_size(s, start, len));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[start])
		sub[i++] = s[start++];
	sub[i] = 0;
	return (sub);
}
