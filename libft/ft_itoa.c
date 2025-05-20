/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:47:43 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/23 14:57:47 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_len(int n)
{
	int	count;

	count = 2;
	if (n == -2147483648)
		return (12);
	else if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n >= 10)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	fill_reversed(int n, char *dst)
{
	if (n == -2147483648)
	{
		fill_reversed(-(n / 10), dst + 1);
		n = -(n % 10);
	}
	if (n >= 10)
	{
		fill_reversed(n / 10, dst + 1);
		n %= 10;
	}
	if (n < 10)
		*dst = (char) n + 48;
}

static char	*reverse_nb(char *reversed_nb, char *ordered_nb)
{
	int	i;
	int	j;

	i = 0;
	if (ordered_nb[0] == '-')
		i++;
	j = ft_strlen(reversed_nb) - 1;
	while (j >= 0)
		ordered_nb[i++] = reversed_nb[j--];
	ordered_nb[i] = 0;
	return (ordered_nb);
}

char	*ft_itoa(int n)
{
	char	*ordered_nb;
	char	reversed_nb[11];

	ordered_nb = (char *) ft_calloc(sizeof(char), get_len(n));
	if (!ordered_nb)
		return (NULL);
	if (n < 0)
	{
		ordered_nb[0] = '-';
		if (n != -2147483648)
			n = -n;
	}
	ft_memset(reversed_nb, 0, 11);
	fill_reversed(n, reversed_nb);
	return (reverse_nb(reversed_nb, ordered_nb));
}
