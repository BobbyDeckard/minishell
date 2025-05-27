/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:40:29 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:40:52 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	free_ast(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	if (node->cmd.args)
		free_string_array(node->cmd.args);
	if (node->cmd.path)
		free(node->cmd.path);
	if (node->file)
		free(node->file);
	if (node->children)
	{
		i = 0;
		while (node->children[i])
		{
			free_ast(node->children[i]);
			i++;
		}
		free(node->children);
	}
	free(node);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static void	print_ast_type(t_ast *node)
{
	if (node->type == NODE_CMD)
	{
		printf("COMMAND:");
		if (node->cmd.args)
		{
			int j = 0;
			while (node->cmd.args[j])
			{
				printf(" %s", node->cmd.args[j]);
				j++;
			}
		}
		printf("\n");
	}
	else if (node->type == NODE_PIPE)
		printf("PIPE |\n");
	else if (node->type == NODE_AND_IF)
		printf("AND &&\n");
	else if (node->type == NODE_OR_IF)
		printf("OR ||\n");
	else if (node->type == NODE_SUBSHELL)
		printf("SUBSHELL ()\n");
	else if (node->type == NODE_REDIR_IN)
		printf("REDIRECTION < %s\n", node->file ? node->file : "NULL");
	else if (node->type == NODE_REDIR_OUT)
		printf("REDIRECTION > %s\n", node->file ? node->file : "NULL");
	else if (node->type == NODE_REDIR_APPEND)
		printf("REDIRECTION >> %s\n", node->file ? node->file : "NULL");
	else if (node->type == NODE_HEREDOC)
		printf("HEREDOC << %s\n", node->file ? node->file : "NULL");
}

void	print_ast(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	print_ast_type(node);
	if (node->children)
	{
		i = 0;
		while (node->children[i])
		{
			print_ast(node->children[i], depth + 1);
			i++;
		}
	}
}

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		len;
	int		i;

	if (!s)
		return (NULL);
	len = 0;
	while (s[len])
		len++;
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
