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

void	handle_sigint_interactive(void)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint_executing(void)
{
	printf("\n");
}

void	handle_sigint_heredoc(t_shell_data *shell)
{
	printf("\n");
	shell->state = INTERACTIVE;
}

static void	process_sigint(t_shell_data *shell)
{
	if (shell->state == INTERACTIVE)
		handle_sigint_interactive();
	else if (shell->state == EXECUTING)
		handle_sigint_executing();
	else if (shell->state == HEREDOC)
		handle_sigint_heredoc(shell);
}

void	handle_signal_in_context(t_shell_data *shell)
{
	if (g_signal_received == SIGINT)
		process_sigint(shell);
	else if (g_signal_received == SIGQUIT)
	{
		if (shell->state == INTERACTIVE)
		{
		}
		else if (shell->state == EXECUTING)
		{
		}
	}
	g_signal_received = 0;
}