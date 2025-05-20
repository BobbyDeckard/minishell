/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:45:46 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 17:35:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	verify_overlap(void *dst, const void *src, size_t len)
{
	size_t	i;

	i = -1;
	while (++i < len)
		if (dst == src + i)
			return (1);
	return (0);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*dst_cast;
	unsigned char	*src_cast;

	if (!src || !dst)
		return (NULL);
	if (!verify_overlap(dst, src, len))
		ft_memcpy(dst, src, len);
	else
	{
		dst_cast = (unsigned char *) dst;
		src_cast = (unsigned char *) src;
		i = len;
		while (--i < len)
			dst_cast[i] = src_cast[i];
	}
	return (dst);
}
