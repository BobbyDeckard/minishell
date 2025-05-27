/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:23:39 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/21 15:13:19 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/* The exit utility shall cause the shell to exit from its current
 * execution environment with the exit status specified by the
 * unsigned decimal integer n.  If the current execution environment
 * is a subshell environment, the shell shall exit from the subshell
 * environment with the specified exit status and continue in the
 * environment from which that subshell environment was invoked;
 * otherwise, the shell utility shall terminate with the specified
 * exit status. If n is specified, but its value is not between 0 and
 * 255 inclusively, the exit status is undefined.
 *
 * A trap on EXIT shall be executed before the shell terminates,
 * except when the exit utility is invoked in that trap itself, in
 * which case the shell shall exit immediately. 
 *
 * The exit status shall be n, if specified, except that the behavior
 * is unspecified if n is not an unsigned decimal integer or is
 * greater than 255. Otherwise, the value shall be the exit value of
 * the last command executed, or zero if no command was executed.
 * When exit is executed in a trap action, the last command is
 * considered to be the command that executed immediately preceding
 * the trap action. */
