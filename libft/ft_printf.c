/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:34:07 by imeulema          #+#    #+#             */
/*   Updated: 2024/11/29 10:59:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	make_conversion(char c, va_list *args, int *ptr)
{
	if (c == 's')
		string_conversion(args, ptr);
	else if (c == 'c')
		char_conversion(args, ptr);
	else if (c == 'd' || c == 'i')
		dec_nbr_conversion(args, ptr);
	else if (c == '%')
	{
		ft_putchar_fd('%', 1);
		(*ptr)++;
	}
	else if (c == 'u')
		uns_conversion(va_arg(*args, unsigned int), ptr);
	else if (c == 'x')
		hex_lo_conversion(args, ptr);
	else if (c == 'X')
		hex_up_conversion(args, ptr);
	else if (c == 'p')
		address_conversion(args, ptr);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		*ptr;
	int		count;

	va_start(args, str);
	i = 0;
	count = 0;
	ptr = &count;
	while (str[i])
	{
		if (str[i] == '%')
			make_conversion(str[++i], &args, ptr);
		else
		{
			ft_putchar_fd(str[i], 1);
			(*ptr)++;
		}
		i++;
	}
	va_end(args);
	return (count);
}
