/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:14:05 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 16:48:32 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// prototype, no way to test it for now
int	exec_subshell(t_ast *node)
{
	char	*path;
	char	*cwd;
	int		status;
	int		pid;
	int		len;

	printf("subshell detected\n");
	status = -1;
	pid = fork();
	if (pid < 0)
		return (fork_error());
	if (pid == 0)
	{
		cwd = getcwd(NULL, 0);
		len = ft_strlen("/minimvp") + ft_strlen(cwd) + 1;
		path = (char *) malloc(len * sizeof(char));
		if (!path)
			malloc_error(node);
		ft_strlcat(path, cwd, len);
		ft_strlcat(path, "/minimvp", len);
		printf("subshell about to be launched with path = %s\n", path);
		if (execve(path, &path, node->root->envp) == -1)
			perror("execve");
		free(path);
		cleanup(node->root);
		exit(FAILURE);
	}
	waitpid(pid, &status, 0);
	printf("subshell stopped running\n");
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}
