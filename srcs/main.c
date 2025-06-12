/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:52:35 by pitran            #+#    #+#             */
/*   Updated: 2025/06/12 16:54:11 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

t_shell_data	g_shell = {
	.envp = NULL,
	.paths = NULL,
	.exit_status = 0,
	.state = INTERACTIVE
};

static char	*get_prompt(void)
{
	return (ft_strdup("minishell> "));
}

static char	**process_command(char *command, char **envp)
{
	t_ast	*ast;

	if (!command || !*command)
		return (envp);
	add_history(command);
	g_shell.state = EXECUTING;
	setup_execution_signals();
	ast = parse_input(command, &g_shell);
	if (ast)
	{
		ast->envp = envp;
		ast->paths = get_paths();
		g_shell.exit_status = exec_ast(ast);
		envp = ast->envp;
		free_ast(ast);
	}
	/* Gérer les signaux reçus pendant l'exécution */
	if (g_signal_received == SIGINT)
	{
		g_shell.exit_status = 130;/* Code de sortie standard pour SIGINT */
		g_signal_received = 0;
	}
	else if (g_signal_received == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		g_shell.exit_status = 131;/* Code de sortie standard pour SIGQUIT */
		g_signal_received = 0;
	}
	return (envp);
}

static int	handle_eof(void)
{
	printf("exit\n");
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	**env_cpy;
	char	*command;
	char	*prompt;
	int		should_exit;

	(void)argc;
	(void)argv;
	g_shell.envp = copy_env(envp);
	g_shell.paths = get_paths();
	g_shell.exit_status = 0;
	g_shell.state = INTERACTIVE;
	should_exit = 0;
	env_cpy = copy_env(envp);
	while (!should_exit)
	{
		g_shell.state = INTERACTIVE;
		setup_interactive_signals();
		prompt = get_prompt();
		command = readline(prompt);
		free(prompt);
		if (!command)
		{
			should_exit = handle_eof();
			continue;
		}
		if (*command)
			env_cpy = process_command(command, env_cpy);
		free(command);
	}
	if (g_shell.envp)
		clean_env_cpy(g_shell.envp, -1);
	if (g_shell.paths)
		clean_env_cpy(g_shell.paths, -1);
	return (g_shell.exit_status);
}
