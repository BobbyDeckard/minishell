/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_lo_conversion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:47:10 by imeulema          #+#    #+#             */
/*   Updated: 2024/11/29 11:00:09 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_putnbr_hex_printf(unsigned int n, int *ptr, char *base)
{
	char	c;

	if (n >= 16)
	{
		ft_putnbr_hex_printf(n / 16, ptr, base);
		n %= 16;
	}
	if (n < 16)
	{
		c = base[n];
		write(1, &c, 1);
		(*ptr)++;
	}
}

void	hex_lo_conversion(va_list *args, int *ptr)
{
	char	base[17];

	ft_strlcpy(base, "0123456789abcdef", 17);
	ft_putnbr_hex_printf(va_arg(*args, long), ptr, base);
}
