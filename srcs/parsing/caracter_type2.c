/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caracter_type2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:57:10 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:41:20 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	is_parenthesis(char c)
{
	if (c == '(' || c == ')')
		return (1);
	return (0);
}

int	is_special_caracter(char c)
{
	if (c == '$' || c == '\\' || c == '#' || c == '\n' || c == '\0')
		return (1);
	return (0);
}
