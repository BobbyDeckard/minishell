/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:13:15 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/27 14:52:03 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	set_exit_status(t_ast *node, int status)
{
	node->root->exit_status = status;
	return (status);
}

int	ft_char_tab_len(char **tab)
{
	int	i;

	if (!tab)
		return (-1);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}
