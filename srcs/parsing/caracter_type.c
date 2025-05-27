/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caracter_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:58:57 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:41:16 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	is_space(char c)
{
	if ((c >= 9 && c <= 12) || (c == 32))
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == 39 || c == 34)
		return (1);
	return (0);
}

int	is_redirection(char c)
{
	if ((c == '>') || (c == '<'))
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '&' || c == '|' || c == ';')
		return (1);
	return (0);
}

int	is_command_char(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	else if (c >= 'A' && c <= 'Z')
		return (1);
	else if (c >= '0' && c <= '9')
		return (1);
	else if (c == '_' || c == '-' || c == '.')
		return (1);
	return (0);
}
