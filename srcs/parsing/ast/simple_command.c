/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:00:00 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:40:49 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_redirection_token(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

t_node_type	token_to_node_type(t_token_type type)
{
	if (type == REDIR_IN)
		return (NODE_REDIR_IN);
	else if (type == REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if (type == REDIR_APPEND)
		return (NODE_REDIR_APPEND);
	else if (type == HEREDOC)
		return (NODE_HEREDOC);
	return (NODE_REDIR_IN);
}

int	count_args(t_token **tokens, int start, int end)
{
	int		count;
	int		i;
	t_token	*current;

	count = 0;
	i = start;
	while (i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (current->type == WORD)
			count++;
		else if (is_redirection_token(current->type))
			i++;
		i++;
	}
	return (count);
}

t_ast	**extract_redirections(t_token **tokens, int start, int end,
	t_ast *root)
{
	t_ast	**redirections;
	int		count;
	int		i;
	int		redir_idx;
	t_token	*current;
	t_token	*target;

	count = count_redirections(tokens, start, end);
	if (count == 0)
		return (NULL);
	redirections = (t_ast **)malloc(sizeof(t_ast *) * (count + 1));
	if (!redirections)
		return (NULL);
	i = start;
	redir_idx = 0;
	while (i <= end && redir_idx < count)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (is_redirection_token(current->type) && i + 1 <= end)
		{
			target = get_token_at_index(tokens, i + 1);
			if (target && target->type == WORD)
			{
				redirections[redir_idx] = create_redirection_node(
						token_to_node_type(current->type),
						ft_strdup(target->content), root);
				if (!redirections[redir_idx])
				{
					while (--redir_idx >= 0)
						free_ast(redirections[redir_idx]);
					free(redirections);
					return (NULL);
				}
				redir_idx++;
			}
			i++;
		}
		i++;
	}
	redirections[redir_idx] = NULL;
	return (redirections);
}

t_ast	*parse_simple_command(t_token **tokens, int start, int end,
	t_ast *root)
{
	t_ast	*node;
	char	**args;
	t_ast	**redirections;

	args = extract_args(tokens, start, end);
	if (!args)
		return (NULL);
	node = create_command_node(args, root);
	if (!node)
	{
		free_string_array(args);
		return (NULL);
	}
	redirections = extract_redirections(tokens, start, end, root);
	if (redirections)
		node->children = redirections;
	return (node);
}

char	**extract_args(t_token **tokens, int start, int end)
{
	char	**args;
	int		count;
	int		i;
	int		arg_idx;
	t_token	*current;

	count = count_args(tokens, start, end);
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = start;
	arg_idx = 0;
	while (i <= end && arg_idx < count)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (current->type == WORD)
		{
			args[arg_idx] = ft_strdup(current->content);
			if (!args[arg_idx])
			{
				free_string_array(args);
				return (NULL);
			}
			arg_idx++;
		}
		else if (is_redirection_token(current->type))
			i++;
		i++;
	}
	args[arg_idx] = NULL;
	return (args);
}

int	count_redirections(t_token **tokens, int start, int end)
{
	int		count;
	int		i;
	t_token	*current;

	count = 0;
	i = start;
	while (i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (is_redirection_token(current->type) && i + 1 <= end)
		{
			current = get_token_at_index(tokens, i + 1);
			if (current && current->type == WORD)
				count++;
		}
		i++;
	}
	return (count);
}
