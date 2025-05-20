/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dec_nbr_conversion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:41:27 by imeulema          #+#    #+#             */
/*   Updated: 2024/11/29 11:00:02 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_putnbr_printf(int n, int *ptr, char c)
{
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		(*ptr) += 11;
		return ;
	}
	else if (n < 0)
	{
		n = -n;
		write(1, "-", 1);
		(*ptr)++;
	}
	if (n >= 10)
	{
		ft_putnbr_printf(n / 10, ptr, c);
		n %= 10;
	}
	if (n < 10)
	{
		c = (char) n + 48;
		write(1, &c, 1);
		(*ptr)++;
	}
}

void	dec_nbr_conversion(va_list *args, int *ptr)
{
	char	c;

	c = 0;
	ft_putnbr_printf(va_arg(*args, int), ptr, c);
}
