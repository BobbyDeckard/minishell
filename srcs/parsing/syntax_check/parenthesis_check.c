

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:34:27 by pitran            #+#    #+#             */
/*   Updated: 2025/03/21 16:25:15 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int count_tokens(t_token **token_list)
{
	t_token	*current;
	int		count;

	count = 0;
	if (!token_list || !*token_list)
		return (0);
	current = *token_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	check_token(t_token *cur, t_token **stack, int *top)
{
	if (cur->type == PAREN_OPEN)
	{
		if (cur->next && cur->next->type == PAREN_CLOSE)
			return ((printf("Error: Empty parenthesis in prompt\n")), 0);
		stack[++(*top)] = cur;
	}
	else if (cur->type == PAREN_CLOSE)
	{
		if (*top < 0)
			return (0);
		(*top)--;
	}
	return (1);
}

int	paren_syntax_is_valid(t_token **token_list)
{
	t_token	**stack;
	int		top;
	t_token	*cur;
	int		res;

	if (!token_list || !*token_list)
		return (1);
	stack = (t_token **)malloc(sizeof(t_token *) * count_tokens(token_list));
	if (!stack)
		return (0);

	t_token *temp = *token_list;
	while (temp) {
//    printf("Token type: %d, content: %s\n", temp->type, temp->content);
    temp = temp->next;
}
	top = -1;
	cur = *token_list;
	res = 1;
	while (cur && res)
	{
		res = check_token(cur, stack, &top);
		cur = cur->next;
	}
	if (top >= 0)
		res = 0;
	free(stack);
	return (res);
}
