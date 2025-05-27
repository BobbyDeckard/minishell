/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:59:12 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:41:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	find_word_len(char *content)
{
	int	i;

	i = 0;
	while (is_command_char(content[i]))
		i++;
	return (i);
}
