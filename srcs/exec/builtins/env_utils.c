/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:06:50 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 16:12:11 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

char	*get_str(const char *name)
{
	char	*str;

	str = getenv(name);
	if (!str)
	{
		perror(name);
		return (NULL);
	}
	return (str);
}
