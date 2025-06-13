/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:34:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/13 16:14:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	**make_new_env(t_ast *node, int size);

char	*cd_error(t_ast	*node)
{
	t_cmd	cmd;
	char	*msg;
	int		len;

	cmd = node->cmd;
	len = ft_strlen(cmd.args[1]);
	msg = (char *) malloc((len + 5) * sizeof(char));
	if (!msg)
		clean_exit(node->root, FAILURE);
	ft_strlcpy(msg, "cd: ", len + 5);
	ft_strlcat(msg, cmd.args[1], len + 5);
	return (msg);
}

void	update_both(t_ast *node, char *prev_wd, int i, int j)
{
	char	*cwd;
	int		len;

	free(prev_wd);
	free(node->root->envp[j]);
	len = ft_strlen(node->root->envp[i]) + 4;
	node->root->envp[j] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[j])
		malloc_error(node);
	ft_strlcat(node->root->envp[j], "OLDPWD=", len);
	ft_strlcat(node->root->envp[j], node->root->envp[i] + 4, len);
	free(node->root->envp[i]);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	node->root->envp[i] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[i])
	{
		free(cwd);
		malloc_error(node);
	}
	ft_strlcat(node->root->envp[i], "PWD=", len);
	ft_strlcat(node->root->envp[i], cwd, len);
	free(cwd);
}

void	update_pwd_create_old(t_ast *node, char *prev_wd, int i)
{
	char	*cwd;
	int		size;
	int		len;

	free(prev_wd);
	size = ft_char_tab_len(node->root->envp) + 1;
	node->root->envp = make_new_env(node, size + 1);
	len = ft_strlen(node->root->envp[i]) + 4;
	node->root->envp[size] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[size])
		malloc_error(node);
	ft_strlcat(node->root->envp[size], "OLDPWD=", len);
	ft_strlcat(node->root->envp[size], node->root->envp[i] + 4, len);
	node->root->envp[size + 1] = NULL;
	free(node->root->envp[i]);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	node->root->envp[i] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[i])
	{
		free(cwd);
		malloc_error(node);
	}
	ft_strlcat(node->root->envp[i], "PWD=", len),
	ft_strlcat(node->root->envp[i], cwd, len);
	free(cwd);
}

void	update_old_create_pwd(t_ast *node, char *prev_wd, int j)
{
	char	*cwd;
	int		size;
	int		len;

	size = ft_char_tab_len(node->root->envp) + 1;
	node->root->envp = make_new_env(node, size + 1);
	len = ft_strlen(prev_wd) + 8;
	free(node->root->envp[j]);
	node->root->envp[j] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[j])
	{
		free(prev_wd);
		malloc_error(node);
	}
	ft_strlcat(node->root->envp[j], "OLDPWD=", len);
	ft_strlcat(node->root->envp[j], prev_wd, len);
	free(prev_wd);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	node->root->envp[size] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[size])
	{
		free(cwd);
		malloc_error(node);
	}
	ft_strlcat(node->root->envp[size], "PWD=", len);
	ft_strlcat(node->root->envp[size], cwd, len);
	node->root->envp[size + 1] = NULL;
	free(cwd);
}

void	create_both(t_ast *node, char *prev_wd)
{
	char	*cwd;
	int		size;
	int		len;

	size = ft_char_tab_len(node->root->envp) + 1;
	node->root->envp = make_new_env(node, size + 2);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	node->root->envp[size] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[size])
	{
		free(prev_wd),
		free(cwd);
		malloc_error(node);
	}
	ft_strlcat(node->root->envp[size], "PWD=", len),
	ft_strlcat(node->root->envp[size], cwd, len);
	free(cwd);
	len = ft_strlen(prev_wd) + 8;
	node->root->envp[++size] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[size])
	{
		free(prev_wd);
		malloc_error(node);
	}
	ft_strlcat(node->root->envp[size], "OLDPWD=", len);
	ft_strlcat(node->root->envp[size], prev_wd, len);
	node->root->envp[size + 1] = NULL;
	free(prev_wd);
}

void	update_pwd(t_ast *node, char *prev_wd)
{
	int	i;
	int	j;

	i = -1;
	while (node->root->envp[++i])
	{
		if (!ft_strncmp(node->root->envp[i], "PWD=", 4))
			break ;
	}
	j = -1;
	while (node->root->envp[++j])
	{
		if (!ft_strncmp(node->root->envp[j], "OLDPWD=", 7))
			break ;
	}
	if (node->root->envp[i] && node->root->envp[j])
		update_both(node, prev_wd, i, j);
	else if (node->root->envp[i])
		update_pwd_create_old(node, prev_wd, i);
	else if (node->root->envp[j])
		update_old_create_pwd(node, prev_wd, j);
	else
		create_both(node, prev_wd);
}

int	cd(t_ast *node)
{
	char	*error;
	char	*cwd;

	// redirs with cd ? is that even possible ?
	if (make_redirs(node) == FAILURE)
		return (set_exit_status(node, FAILURE));
	cwd = getcwd(NULL, 0);
	if (chdir(node->cmd.args[1]) < 0)
	{
		error = cd_error(node);
		perror(error);
		free(error);
		free(cwd);
		close_redirs(node->cmd);
		unlink_heredoc(node);
		return (set_exit_status(node, FAILURE));
	}
	update_pwd(node, cwd);
	free(cwd);
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, SUCCESS));
}
