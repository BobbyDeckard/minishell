/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:34:55 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/01 15:41:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	make_nb(const char *str, int i, int m)
{
	int	nb;

	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + str[i++] - 48;
	if (m == 1)
		nb = -nb;
	return (nb);
}

int	ft_atoi(const char *str)
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
