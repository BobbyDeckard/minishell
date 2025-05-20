/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:59:03 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/18 14:11:25 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	size_t	i;
	char	*ptr;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	ptr = (char *) malloc((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	i = -1;
	while (++i < len)
		ptr[i] = s1[i];
	ptr[i] = 0;
	return (ptr);
}
