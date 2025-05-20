/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:52:34 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 16:24:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*p;

	if (!s)
		return (NULL);
	p = NULL;
	while (*s)
	{
		if (*s == (char ) c)
			p = (char *) s;
		s++;
	}
	if ((char) c == 0)
		p = (char *) s;
	return (p);
}
