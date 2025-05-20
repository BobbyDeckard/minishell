/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 15:34:21 by imeulema          #+#    #+#             */
/*   Updated: 2024/10/20 15:36:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*next_node;

	next_node = *lst;
	while (*lst)
	{
		next_node = (*lst)-> next;
		ft_lstdelone(*lst, del);
		*lst = next_node;
	}
	free(*lst);
	*lst = NULL;
}
