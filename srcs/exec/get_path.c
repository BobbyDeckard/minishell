/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 00:37:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/04/10 14:21:07 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	found_path(t_cmd *cmd, char *path)
{
	cmd->path = ft_strdup(path);
	free(path);
}

// itère dans le tableau de strings récupéré de l'environnement jusqu'à trouver
// le fichier exécutable correspondant à la commande demandée
// si aucun fichier exécutable n'a été trouvé après avoir itéré dans tout le
// tableau, un message d'erreur est imprimé avec le nom de la commande donnée
// par l'utilisateur
void	get_cmd_path(t_cmd *cmd, char **paths)
{
	char	*full_path;
	char	*name;
	int		i;

	name = cmd->args[0];
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin(paths[i], name);
		if (access(full_path, F_OK) == 0)
			return (found_path(cmd, full_path));
		free(full_path);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": command not found\n", 2);
}

char	**get_paths(void)
{
	char	**paths;
	char	*all_paths;

	all_paths = getenv("PATH");
	paths = ft_split_path(all_paths, ':');	// need to secure those functions
	return (paths);
}
