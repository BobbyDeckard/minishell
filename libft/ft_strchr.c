/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:48:25 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 16:22:07 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*p;

	while (*s)
	{
		if (*s == (char) c)
		{
			p = (char *) s;
			return (p);
		}
		s++;
	}
	if ((char) c == 0)
	{
		p = (char *) s;
		return (p);
	}
	return (NULL);
}
