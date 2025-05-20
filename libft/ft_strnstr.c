/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:11:46 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/01 15:33:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (!haystack || !needle)
		return (NULL);
	else if (!needle[0])
		return ((char *) haystack);
	i = 0;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i] && needle[j] && haystack[i] == needle[j])
		{
			i++;
			j++;
		}
		if (!needle[j] && i <= len)
			return ((char *) &haystack[i - j]);
		i -= j;
		i++;
	}
	return (NULL);
}
