/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:33:30 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 17:38:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	src_len;

	if (!dst || !src)
		return (0);
	src_len = ft_strlen(src);
	i = ft_strlen(dst);
	if (i >= dstsize)
		return (dstsize + src_len);
	j = -1;
	while (++j + i + 1 < dstsize && src[j])
		dst[i + j] = src[j];
	dst[i + j] = 0;
	return (i + src_len);
}
