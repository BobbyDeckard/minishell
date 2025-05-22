/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:58:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/22 17:16:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	run_debug(char **envp, char *command)
{
	t_ast	*ast;
	int		preset;

	preset = ft_atoi(command);
	if (preset > 19 || preset == 17)
	{
		print_options();
		free(command);
	}
	else
	{
		free(command);
		printf("Debug preset %d called\n", preset);
		print_cmd(preset);
		ast = make_ast(preset);
		ast->paths = get_paths();
		ast->envp = envp;
		exec_ast(ast);
		cleanup(ast);
	}
}
