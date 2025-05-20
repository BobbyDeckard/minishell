/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:31:07 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 16:32:41 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dst_cast;
	unsigned char	*src_cast;

	if (!src || !dst)
		return (NULL);
	dst_cast = (unsigned char *) dst;
	src_cast = (unsigned char *) src;
	i = -1;
	while (++i < n)
		dst_cast[i] = src_cast[i];
	return (dst);
}
