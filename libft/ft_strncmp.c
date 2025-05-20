/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:57:06 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 17:38:59 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;

	if (!n || !s1 || !s2)
		return (-1);
	i = -1;
	while (++i < n && (s1[i] || s2[i]))
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i]
			- ((unsigned char *)s2)[i]);
	if (i > n && (!s1[i] || !s2[i]))
		return (ft_strlen(s1) - ft_strlen(s2));
	return (0);
}
