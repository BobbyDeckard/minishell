/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/XX/XX XX:XX:XX by student          #+#    #+#             */
/*   Updated: 2024/XX/XX XX:XX:XX by student         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* ========== INITIALISATION COMPLÈTE ========== */
t_shell_data g_shell = {
	.envp = NULL,
	.paths = NULL,
	.exit_status = 0,
	.state = INTERACTIVE
};

/* ========== FONCTION PRINCIPALE CORRIGÉE ========== */
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
	
	/* ========== CONFIGURATION INITIALE DES SIGNAUX ========== */
	setup_interactive_signals();
	
	while (1)
	{
		/* ========== RESET DU SIGNAL AVANT CHAQUE ITERATION ========== */
		g_signal_received = 0;
		g_shell.state = INTERACTIVE;
		
		/* ========== LECTURE DE LA COMMANDE ========== */
		command = readline("Petit coquillage > ");
		
		/* ========== GESTION CTRL-D (EOF) ========== */
		if (!command)
		{
			printf("exit\n");
			break;
		}
	
		
		/* ========== TRAITEMENT DES COMMANDES NON-VIDES ========== */
		if (command && *command)
		{
			add_history(command);
			
			/* ========== MODE EXÉCUTION ========== */
			g_shell.state = EXECUTING;
			setup_execution_signals();
			
			/* ========== PARSING ET EXÉCUTION ========== */
			t_ast *ast = parse_input(command, &g_shell);
			if (ast)
			{
				g_shell.exit_status = exec_ast(ast);
				free_ast(ast);
			}
			
			/* ========== RETOUR AU MODE INTERACTIF ========== */
			g_shell.state = INTERACTIVE;
			setup_interactive_signals();
			
			/* ========== GESTION SIGNAUX POST-EXÉCUTION ========== */
			if (g_signal_received)
			{
				g_signal_received = 0;  /* Reset après exécution */
			}
		}
		
		free(command);
	}
	
	return (g_shell.exit_status);
}