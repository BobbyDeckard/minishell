/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:00:43 by pitran            #+#    #+#             */
/*   Updated: 2025/05/20 14:21:09 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell_data	g_shell = {NULL, NULL, 0};

int	main(int argc, char **argv, char **envp)
{
	char	*command;

	(void)argc;
	(void)argv;
	g_shell.envp = envp;
	g_shell.exit_status = 0;
	while (1)
	{
		command = readline("Petit coquillage > ");
		if (!command)
			break ;
		if (*command)
		{
			add_history(command);
			parse_input(command);
		}
		free(command);
	}
	return (0);
}