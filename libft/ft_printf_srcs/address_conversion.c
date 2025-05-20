/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address_conversion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:56:10 by imeulema          #+#    #+#             */
/*   Updated: 2024/11/29 10:59:41 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_putaddress_printf(unsigned long address, int *ptr, char *base)
{
	char	c;

	if (address >= 16)
	{
		ft_putaddress_printf(address / 16, ptr, base);
		address %= 16;
	}
	if (address < 16)
	{
		c = base[address];
		write(1, &c, 1);
		(*ptr)++;
	}
}

void	address_conversion(va_list *args, int *ptr)
{
	void			*ptr_to_print;
	unsigned long	address;
	char			base[17];

	write(1, "0x", 2);
	(*ptr) += 2;
	ptr_to_print = va_arg(*args, void *);
	address = (unsigned long) ptr_to_print;
	ft_strlcpy(base, "0123456789abcdef", 17);
	ft_putaddress_printf(address, ptr, base);
}
