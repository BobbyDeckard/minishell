/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:00:00 by pitran            #+#    #+#             */
/*   Updated: 2025/06/12 16:49:30 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	handle_sigint_interactive(void)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigint_executing(void)
{
	write(STDOUT_FILENO, "\n", 1);
}

void	handle_sigint_heredoc(t_shell_data *shell)
{
	write(STDOUT_FILENO, "\n", 1);
	shell->state = INTERACTIVE;
}

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
		if (shell->state == EXECUTING)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
	g_signal_received = 0;
}
