/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 11:33:39 by imeulema          #+#    #+#             */
/*   Updated: 2025/02/25 11:33:41 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	make_sign(double nb, int m)
{
	if (m)
		nb *= -1;
	return (nb);
}

static double	make_nb(const char *str, int i, int m)
{
	double	integers;
	double	decimals;
	double	power10;

	integers = 0;
	while (str[i] >= '0' && str[i] <= '9')
		integers = integers * 10 + str[i++] - 48;
	if (str[i] == '.' || str[i] == ',')
		i++;
	else
		return (make_sign(integers, m));
	decimals = 0;
	power10 = 10;
	while (str[i] >= '0' && str[i] <= '9')
	{
		decimals += (str[i++] - 48) / power10;
		power10 *= 10;
	}
	return (make_sign(integers + decimals, m));
}

double	ft_atod(const char *str)
{
	int	i;
	int	p;
	int	m;

	i = 0;
	p = 0;
	m = 0;
	if (!str)
		return (0);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '+' && m == 0 && p == 0)
			p++;
		else if (str[i] == '-' && m == 0 && p == 0)
			m++;
		else
			return (0);
		i++;
	}
	return (make_nb(str, i, m));
}
