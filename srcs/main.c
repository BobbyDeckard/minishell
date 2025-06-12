/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:47:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/27 14:41:54 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	run_debug(char **envp, char *command);


int main(int ac, char **av, char **envp)
{
	t_ast	*ast;
	char	**env_cpy;
	char	*command;
	char	*cwd;

	(void) ac;
	(void) av;
	
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
//					printf("\nenv_cpy at beginning of loop:\n");
//					int x = -1;
//					while (env_cpy[++x])
//						printf("env_cpy[%d]: %s\n", x, env_cpy[x]);
					ast->paths = get_paths();
					ast->envp = env_cpy;
//					printf("\nast->envp after copying env_cpy:\n");
//					x = -1;
//					while (ast->envp[++x])
//						printf("ast->envp[%d]: %s\n", x, ast->envp[x]);
					exec_ast(ast);
//					printf("\nenvp right after execution:\n");
//					x = -1;
//					while (ast->envp[++x])
//						printf("envp[%d]: %s\n", x, ast->envp[x]);
					env_cpy = ast->envp;
//					printf("\nenv_cpy in preparation for next command:\n");
//					x = -1;
//					while (env_cpy[++x])
//						printf("env_cpy[%d]: %s\n", x, env_cpy[x]);
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
