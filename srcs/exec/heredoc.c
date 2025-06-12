/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:37:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 20:12:18 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// << word
// reads input until it finds a line containing only word with no trailing blanks

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
		j = 64;
		while (++j < 128)
		{
			name[i] = j;
			if (check_and_open(name, node, cmd))
				return (SUCCESS);
		}
	}
	return (FAILURE);
}

/* Signal handler spécial pour heredoc qui ferme STDIN */
void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	close(STDIN_FILENO);
}

void	make_heredoc(t_ast *node, t_cmd *cmd)
{
	char	*line;
	char	*delimiter;
	int		len;

	delimiter = node->file;
	len = ft_strlen(delimiter) + 1;
//	if (file_namer_2000(node, cmd) == FAILURE)
//		return ;
	if (!check_and_open("temp", node, cmd))
	{
		if (!check_and_open("am1itlakjndlejbgfoaknmifubpqms", node, cmd))
			return ;
	}
	while (1)
	{
		line = readline("> ");
		if (!ft_strncmp(line, delimiter, len))
			break ;
		ft_putstr_fd(line, cmd->fd_in);
		ft_putchar_fd('\n', cmd->fd_in);
		free(line);
	}
	free(line);
	close(cmd->fd_in);
	cmd->fd_in = open(node->file, O_RDONLY);
	if (cmd->fd_in < 0)
		perror(node->file);
}

/*
void	make_heredoc(t_ast *node, t_cmd *cmd)
{
	char				*line;
	char				*delimiter;
	int					len;
	int					stdin_backup;
	struct sigaction	old_action;
	struct sigaction	new_action;

	delimiter = node->file;
	len = ft_strlen(delimiter);
	
	if (!check_and_open("temp", node, cmd))
		return ;
	
	*//* Backup STDIN et setup signal handler spécial *//*
	stdin_backup = dup(STDIN_FILENO);
	new_action.sa_handler = sigint_heredoc_handler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;
	sigaction(SIGINT, &new_action, &old_action);
	
	g_shell.state = HEREDOC_MODE;
	
	while (1)
	{
		line = readline("> ");
		
		*//* Si readline retourne NULL et STDIN n'est plus un terminal, 
		   c'est qu'on a été interrompu par SIGINT *//*
		if (!line)
		{
			if (!isatty(STDIN_FILENO))
			{
				*//* Interrupted by signal *//*
				dup2(stdin_backup, STDIN_FILENO);
				close(stdin_backup);
				close(cmd->fd_in);
				unlink(node->file);
				cmd->fd_in = -1;
				write(STDOUT_FILENO, "\n", 1);
			}
			else
			{
				*//* Normal EOF (Ctrl-D) *//*
				printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			}
			break ;
		}
		
		*//* Vérifier délimiteur *//*
		if (!ft_strncmp(line, delimiter, len + 1))
		{
			free(line);
			break ;
		}
		
		ft_putstr_fd(line, cmd->fd_in);
		ft_putchar_fd('\n', cmd->fd_in);
		free(line);
	}
	
	*//* Restore signal handler et STDIN *//*
	sigaction(SIGINT, &old_action, NULL);
	if (stdin_backup >= 0)
		close(stdin_backup);
	
	*//* Si pas interrompu, configurer le fd pour lecture *//*
	if (cmd->fd_in != -1)
	{
		close(cmd->fd_in);
		cmd->fd_in = open(node->file, O_RDONLY);
		if (cmd->fd_in < 0)
			perror(node->file);
	}
	
	g_shell.state = INTERACTIVE;
	g_signal_received = 0;
}
*/
