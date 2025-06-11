/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:37:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/11 17:08:06 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

extern t_shell_data	g_shell;

void	unlink_heredoc(t_ast *node)
{
	if (node->children && node->children[0]->type == NODE_HEREDOC)
		unlink(node->children[0]->file);
}

int	check_and_open(char *name, t_ast *node, t_cmd *cmd)
{
	if (access(name, F_OK) == 0)
		return (0);
	cmd->fd_in = open(name, O_WRONLY | O_CREAT, 0644);
	node->file = name;
	if (cmd->fd_in < 0)
		perror(node->file);
	return (1);
}

int	file_namer_2000(t_ast *node, t_cmd *cmd)
{
	char	name[128];
	int		i;
	int		j;

	i = -1;
	while (++i < 128)
		name[i] = 0;
	i = -1;
	while (++i < 128)
	{
		j = 32;
		while (++j < 128)
		{
			name[i] = j;
			if (check_and_open(name, node, cmd))
				return (SUCCESS);
		}
	}
	return (FAILURE);
}

void	make_heredoc(t_ast *node, t_cmd *cmd)
{
	char	*line;
	char	*delimiter;
	int		len;

	delimiter = node->file;
	len = ft_strlen(delimiter);
	
	if (!check_and_open("temp", node, cmd))
		return ;
	
	g_shell.state = HEREDOC_MODE;
	setup_heredoc_signals();  /* Setup special heredoc signals */
	
	while (1)
	{
		line = readline("> ");
		
		/* Check for SIGINT or EOF */
		if (g_signal_received == SIGINT || !line)
		{
			if (line)
				free(line);
			/* Cleanup on interruption */
			close(cmd->fd_in);
			unlink(node->file);
			cmd->fd_in = -1;
			g_shell.state = INTERACTIVE;
			g_signal_received = 0;
			setup_interactive_signals();  /* Restore normal signals */
			return ;
		}
		
		/* Check delimiter */
		if (!ft_strncmp(line, delimiter, len + 1))
		{
			free(line);
			break ;
		}
		
		ft_putstr_fd(line, cmd->fd_in);
		ft_putchar_fd('\n', cmd->fd_in);
		free(line);
	}
	
	close(cmd->fd_in);
	cmd->fd_in = open(node->file, O_RDONLY);
	if (cmd->fd_in < 0)
		perror(node->file);
	
	g_shell.state = INTERACTIVE;
	setup_interactive_signals();  /* Restore normal signals */
}