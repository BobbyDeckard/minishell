/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:54:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/16 21:12:16 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <limits.h>			// autorisé ?
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>			// autorisé ?
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include <signal.h>
#include "structs.h"
#include "signals.h"
# include "parsing.h"
# include "exec.h"
# include "../libft/libft.h"

extern volatile sig_atomic_t g_signal_received;

extern void	rl_replace_line(const char *str, int i);	// potentiellement à enlever pour l'eval, étant donné qu'on sera sur linux

#endif
