/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/XX/XX XX:XX:XX by student          #+#    #+#             */
/*   Updated: 2024/XX/XX XX:XX:XX by student         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* Handle SIGINT in interactive mode */
void	handle_sigint_interactive(void)
{
	/* Use write() instead of printf() - it's signal-safe */
	write(STDOUT_FILENO, "\n", 1);
	
	/* Reset readline state */
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* Handle SIGINT during execution */
void	handle_sigint_executing(void)
{
	write(STDOUT_FILENO, "\n", 1);
	/* Don't redisplay prompt during execution */
}

/* Handle SIGINT during heredoc */
void	handle_sigint_heredoc(t_shell_data *shell)
{
	write(STDOUT_FILENO, "\n", 1);
	shell->state = INTERACTIVE;
	/* Exit heredoc mode gracefully */
}

/* Process signals based on current shell state */
void	handle_signal_in_context(t_shell_data *shell)
{
	if (g_signal_received == SIGINT)
	{
		if (shell->state == INTERACTIVE)
			handle_sigint_interactive();
		else if (shell->state == EXECUTING)
			handle_sigint_executing();
		else if (shell->state == HEREDOC_MODE)
			handle_sigint_heredoc(shell);
	}
	else if (g_signal_received == SIGQUIT)
	{
		/* SIGQUIT handling based on state */
		if (shell->state == EXECUTING)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		}
		/* In interactive mode, SIGQUIT should be ignored */
	}
	
	g_signal_received = 0;  /* Reset after handling */
}