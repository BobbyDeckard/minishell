/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:15:46 by pitran            #+#    #+#             */
/*   Updated: 2025/05/27 14:40:46 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	get_operator_precedence(t_token_type type)
{
	if (type == AND || type == OR)
		return (1);
	else if (type == PIPE)
		return (2);
	return (3);
}

t_node_type	token_type_to_node_type(t_token_type type)
{
	if (type == PIPE)
		return (NODE_PIPE);
	else if (type == AND)
		return (NODE_AND_IF);
	else if (type == OR)
		return (NODE_OR_IF);
	return (NODE_CMD);
}

int	find_lowest_precedence_op(t_token **tokens, int start, int end)
{
	int		result;
	int		lowest_precedence;
	int		current_precedence;
	int		paren_level;
	int		i;
	t_token	*current;

	result = -1;
	lowest_precedence = INT_MAX;
	paren_level = 0;
	i = start;
	while (i <= end)
	{
		current = get_token_at_index(tokens, i);
		if (!current)
			break ;
		if (current->type == PAREN_OPEN)
			paren_level++;
		else if (current->type == PAREN_CLOSE)
			paren_level--;
		if (paren_level == 0 && (current->type == PIPE
				|| current->type == AND || current->type == OR))
		{
			current_precedence = get_operator_precedence(current->type);
			if (current_precedence <= lowest_precedence)
			{
				lowest_precedence = current_precedence;
				result = i;
			}
		}
		i++;
	}
	return (result);
}

t_ast	*parse_command_line(t_token **tokens, int start, int end, t_ast *root)
{
	int			op_pos;
	t_node_type	node_type;
	int			closing;
	t_token		*current;

	if (start > end)
		return (NULL);
	if (matching_parentheses(tokens, start, end))
		return (parse_command_line(tokens, start + 1, end - 1, root));
	op_pos = find_lowest_precedence_op(tokens, start, end);
	if (op_pos == -1)
	{
		current = get_token_at_index(tokens, start);
		if (start < end && current && current->type == PAREN_OPEN)
		{
			closing = find_matching_parenthesis(tokens, start, end);
			if (closing > start && closing <= end)
				return (create_subshell_node(parse_command_line(tokens,
							start + 1, closing - 1, root), root));
		}
		return (parse_simple_command(tokens, start, end, root));
	}
	current = get_token_at_index(tokens, op_pos);
	if (!current)
		return (parse_simple_command(tokens, start, end, root));
	node_type = token_type_to_node_type(current->type);
	return (create_operator_node(node_type,
			parse_command_line(tokens, start, op_pos - 1, root),
			parse_command_line(tokens, op_pos + 1, end, root), root));
}
