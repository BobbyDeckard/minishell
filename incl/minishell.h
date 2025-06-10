/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:54:26 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/10 13:15:31 by pitran           ###   ########.fr       */
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
# include "parsing.h"
# include "exec.h"
# include "../libft/libft.h"
#include "signals.h"

extern volatile sig_atomic_t g_signal_received;

# ifndef SUCCESS
#  define SUCCESS 0 /*pas sur que cela respecte les contraintes du sujet */
# endif

# ifndef FAILURE
#  define FAILURE 1
# endif

#endif
