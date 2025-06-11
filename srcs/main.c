/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:52:35 by pitran            #+#    #+#             */
/*   Updated: 2025/06/11 12:52:42 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../incl/minishell.h"

/* ========== INITIALISATION COMPLÈTE ========== */
t_shell_data g_shell = {
	.envp = NULL,
	.paths = NULL,
	.exit_status = 0,
	.state = INTERACTIVE    /* ← INITIALISER LE NOUVEAU CHAMP */
};

int	main(int argc, char **argv, char **envp)
{
	char	*command;

	(void)argc;
	(void)argv;
	
	/* ========== INITIALISATION ========== */
	g_shell.envp = envp;
	g_shell.paths = NULL;
	g_shell.exit_status = 0;
	g_shell.state = INTERACTIVE;
	
	while (1)
	{
		/* ========== MODE INTERACTIF ========== */
		g_signal_received = 0;
		g_shell.state = INTERACTIVE;
		setup_interactive_signals();
		
		command = readline("Petit coquillage > ");
		
		/* ========== GESTION SIGNAUX ========== */
		if (g_signal_received)
		{
			handle_signal_in_context(&g_shell);
			if (command)
				free(command);
			continue;
		}
		
		/* ========== GESTION EOF (Ctrl-D) ========== */
		if (!command)
		{
			printf("exit\n");
			break;
		}
		
		if (*command)
		{
			add_history(command);
			
			/* ========== MODE EXÉCUTION ========== */
			g_shell.state = EXECUTING;
			setup_execution_signals();
			
			/* Fonction parse existante */
			t_ast *ast = parse_input(command, &g_shell);
			if (ast)
			{
				/* Fonction exec existante */
				exec_ast(ast);  /* Ajouter &g_shell si nécessaire */
				free_ast(ast);
			}
			
			/* ========== VÉRIFIER SIGNAUX POST-EXEC ========== */
			if (g_signal_received)
				handle_signal_in_context(&g_shell);
		}
		free(command);
	}
	return (g_shell.exit_status);
}