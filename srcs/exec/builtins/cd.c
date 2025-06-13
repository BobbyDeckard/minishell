/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:34:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 20:13:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

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

void	update_both(t_ast *node, int i, int j)
{
	char	*cwd;
	int		len;

//	printf("Entering updating func for both variables\n");
	free(node->root->envp[j]);
	len = ft_strlen(node->root->envp[i]) + 4;
	node->root->envp[j] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[j])
		malloc_error(node);
	ft_strlcat(node->root->envp[j], "OLDPWD=", len);
	ft_strlcat(node->root->envp[j], node->root->envp[i] + 4, len);
//	printf("Concatenated new OLDPWD: %s\n", node->root->envp[j]);
	free(node->root->envp[i]);
	cwd = getcwd(NULL, 0);
	len = ft_strlen(cwd) + 5;
	node->root->envp[i] = (char *) malloc(len * sizeof(char));
	if (!node->root->envp[i])
		malloc_error(node);
	ft_strlcat(node->root->envp[i], "PWD=", len);
	ft_strlcat(node->root->envp[i], cwd, len);
//	printf("Concatenated new PWD: %s\n", node->root->envp[i]);
	free(cwd);
}

void	update_pwd_create_old(t_ast *node, int i)
{
	char	**new_env;
	char	*cwd;
	int		len;

	len = ft_char_tab_len(node->root->envp) + 2;
	new_env = (char **) malloc(len * sizeof(char *));
	if (!new_env)
		malloc_error(node);

}

void	update_pwd(t_ast *node)
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
		update_both(node, i, j);
	else if (node->root->envp[i])
		update_pwd_create_old(node, i);
	else if (node->root->envp[j])
		update_old_create_pwd(node, j);
	else
		create_both(node);
}

int	cd(t_ast *node)
{
	char	*error;

	// redirs with cd ? is that even possible ?
	if (make_redirs(node) == FAILURE)
		return (set_exit_status(node, FAILURE));
	if (chdir(node->cmd.args[1]) < 0)
	{
		error = cd_error(node);
		perror(error);
		free(error);
		close_redirs(node->cmd);
		unlink_heredoc(node);
		return (set_exit_status(node, FAILURE));
	}
	update_pwd(node);
	close_redirs(node->cmd);
	unlink_heredoc(node);
//	printf("About to exit cd builtin\n");
//	int x = -1;
//	while (node->root->envp[++x])
//		printf("envp[%d]: %s\n", x, node->root->envp[x]);
	return (set_exit_status(node, SUCCESS));
}
