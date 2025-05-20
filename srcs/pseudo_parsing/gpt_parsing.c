/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpt_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:14:23 by imeulema          #+#    #+#             */
/*   Updated: 2025/04/15 10:15:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

#include <string.h>

bool	is_operator(const char *token)
{
	return (!strcmp(token, "|") || !strcmp(token, "&&") || !strcmp(token, "||"));
}

char	**tokenize(const char *input)
{
	return ft_split(input, ' ');
}

t_ast	*parse_command(char **tokens, int *i)
{
	t_ast	*node = malloc(sizeof(t_ast));
	node->type = NODE_CMD;
	node->cmd.args = malloc(sizeof(char *) * 10); // Up to 10 args for demo
	int	j = 0;
	while (tokens[*i] && !is_operator(tokens[*i]))
		node->cmd.args[j++] = strdup(tokens[(*i)++]);
	node->cmd.args[j] = NULL;
	node->cmd.fd_in = STDIN_FILENO;
	node->cmd.fd_out = STDOUT_FILENO;
	node->children = NULL;
	node->file = NULL;
	return node;
}

t_ast	*parse_pipe(char **tokens, int *i)
{
	t_ast	*left = parse_command(tokens, i);
	while (tokens[*i] && strcmp(tokens[*i], "|") == 0)
	{
		(*i)++;
		t_ast *right = parse_command(tokens, i);
		t_ast *pipe = malloc(sizeof(t_ast));
		pipe->type = NODE_PIPE;
		pipe->children = malloc(sizeof(t_ast *) * 3);
		pipe->children[0] = left;
		pipe->children[1] = right;
		pipe->children[2] = NULL;
		left = pipe;
	}
	return left;
}

t_ast	*parse_and_or(char **tokens, int *i)
{
	t_ast	*left = parse_pipe(tokens, i);
	while (tokens[*i] && (strcmp(tokens[*i], "&&") == 0 || strcmp(tokens[*i], "||") == 0))
	{
		int	type = strcmp(tokens[*i], "&&") == 0 ? NODE_AND_IF : NODE_OR_IF;
		(*i)++;
		t_ast *right = parse_pipe(tokens, i);
		t_ast *logic = malloc(sizeof(t_ast));
		logic->type = type;
		logic->children = malloc(sizeof(t_ast *) * 3);
		logic->children[0] = left;
		logic->children[1] = right;
		logic->children[2] = NULL;
		left = logic;
	}
	return left;
}

t_ast	*make_ast_from_input(const char *input)
{
	char	**tokens = tokenize(input);
	int		i = 0;
	t_ast	*root = parse_and_or(tokens, &i);
	// Free token array if needed
	return root;
}
