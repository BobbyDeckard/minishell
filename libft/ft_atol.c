/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 11:33:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/02/25 11:33:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static long	make_nb(const char *str, int i, int m)
{
	long	nb;

	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + str[i++] - 48;
	if (m == 1)
		nb = -nb;
	return (nb);
}

long	ft_atol(const char *str)
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
