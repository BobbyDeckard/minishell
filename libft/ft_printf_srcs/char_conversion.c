/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_conversion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:42:55 by imeulema          #+#    #+#             */
/*   Updated: 2024/11/29 10:59:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	char_conversion(va_list *args, int *ptr)
{
	ft_putchar_fd(va_arg(*args, int), 1);
	(*ptr)++;
}
