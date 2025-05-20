/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:01:15 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/01 15:05:20 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	unsigned char		needle;
	size_t				i;

	if (!s)
		return (NULL);
	i = -1;
	ptr = (unsigned char *) s;
	needle = (unsigned char) c;
	while (++i < n)
	{
		if (ptr[i] == needle)
		{
			s += i;
			return ((void *) s);
		}
	}
	return (NULL);
}
