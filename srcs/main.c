/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:47:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/23 11:48:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	run_debug(char **envp, char *command);

t_shell_data	g_shell = {NULL, NULL, 0};

int main(int ac, char **av, char **envp)
{
	t_ast	*ast;
	char	**env_cpy;
	char	*command;
	char	*cwd;

	(void) ac;
	(void) av;
	
	g_shell.envp = envp;					// à retirer
	g_shell.exit_status = 0;				// à retirer
	env_cpy = copy_env(envp);
	while (1)
	{
		cwd = make_cwd();
		command = readline(cwd);
		free(cwd);
		if (command)
		{
			if (ft_isdigit(*command))
				run_debug(envp, command);
			else
			{
				add_history(command);
				ast = parse_input(command);
				free(command);
				if (ast)
				{
					ast->paths = get_paths();
					ast->envp = env_cpy;
					exec_ast(ast);
					env_cpy = ast->envp;
					cleanup(ast->root);
				}
			}
		}
		else
			break ;
	}
	clean_env_cpy(env_cpy, -1);
	return (0);
}
