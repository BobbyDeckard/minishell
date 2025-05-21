/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:47:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/21 11:02:19 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	run_debug(char **envp, char *command);

t_shell_data	g_shell = {NULL, NULL, 0};

/* Real main */
int main(int ac, char **av, char **envp)
{
	t_ast	*ast;
	char	*command;
	char	*cwd;

	(void) ac;
	(void) av;
	
	g_shell.envp = envp;
	g_shell.exit_status = 0;
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
					exec_ast(ast);
					cleanup(ast->root);
				}
			}
		}
		else
			break;
	}
	return (0);
}

/* Debug main */
/*
int	main(int ac, char **av, char **envp)
{
	t_ast	*ast;
	char	*command;
	char	*cwd;
	int		preset;

	(void) ac;
	(void) av;

	while (1)
	{
		cwd = make_cwd();
		command = readline(cwd);
		free(cwd);
		if (command)
		{
			preset = ft_atoi(command);
			if (preset > 18 || !ft_isdigit(*command))
			{
				print_options();
				free(command);
			}
			else
			{
				free(command);
				print_cmd(preset);
				ast = make_ast(preset);
				ast->paths = get_paths();
				ast->envp = envp;
				exec_ast(ast);
				cleanup(ast);
			}
		}
		else
			break ;
	}
	return (0);
}
*/
