/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   presets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:07:27 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/22 17:14:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"
#include <unistd.h>

char **make_args(int count, ...) {
	// DO NOT SUBMIT THIS FILE
    va_list args;
    va_start(args, count);

    char **argv = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++)
        argv[i] = ft_strdup(va_arg(args, char *));
    argv[count] = NULL;

    va_end(args);
    return argv;
}

t_ast	*make_simple_pipe(void)
{
	// cat file | grep foo
	
	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "grep", "foo");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = cmd1;
	pipe->children[1] = cmd2;
	pipe->children[2] = NULL;
	pipe->file = NULL;
	
	set_root_node(pipe, pipe);

	return (pipe);
}

t_ast	*make_multi_pipe(void)
{
	// cat file | grep foo | grep bar | wc -l

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "grep", "foo");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*cmd3;
	cmd3 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd3)
		exit(1);
	cmd3->type = NODE_CMD;
	cmd3->cmd.args = make_args(2, "grep", "bar");
	cmd3->cmd.fd_in = STDIN_FILENO;
	cmd3->cmd.fd_out = STDOUT_FILENO;
	cmd3->children = NULL;
	cmd3->file = NULL;

	t_ast	*cmd4;
	cmd4 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd4)
		exit(1);
	cmd4->type = NODE_CMD;
	cmd4->cmd.args = make_args(2, "wc", "-l");
	cmd4->cmd.fd_in = STDIN_FILENO;
	cmd4->cmd.fd_out = STDOUT_FILENO;
	cmd4->children = NULL;
	cmd4->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(5 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = cmd1;
	pipe->children[1] = cmd2;
	pipe->children[2] = cmd3;
	pipe->children[3] = cmd4;
	pipe->children[4] = NULL;
	pipe->file = NULL;
	
	set_root_node(pipe, pipe);

	return (pipe);
}

t_ast	*make_double_redirs(void)
{
	// < file wc -l > outfile

	t_ast	*redir_in;
	redir_in = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_in)
		exit(1);
	redir_in->type = NODE_REDIR_IN;
	redir_in->cmd.args = NULL;
	redir_in->children = NULL;
	redir_in->file = "file";

	t_ast	*redir_out;
	redir_out = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_out)
		exit(1);
	redir_out->type = NODE_REDIR_OUT;
	redir_out->cmd.args = NULL;
	redir_out->children = NULL;
	redir_out->file = "outfile";

	t_ast	*cmd;
	cmd = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd)
		exit(1);
	cmd->type = NODE_CMD;
	cmd->cmd.args = make_args(2, "wc", "-l");
	cmd->cmd.fd_in = STDIN_FILENO;
	cmd->cmd.fd_out = STDOUT_FILENO;
	cmd->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!cmd->children)
		exit(1);
	cmd->children[0] = redir_in;
	cmd->children[1] = redir_out;
	cmd->children[2] = NULL;
	cmd->file = NULL;

	set_root_node(cmd, cmd);

	return (cmd);
}

t_ast	*make_double_redirs_simple_pipe(void)
{
	// < file grep foo | wc -l > outfile

	t_ast	*redir_in;
	redir_in = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_in)
		exit(1);
	redir_in->type = NODE_REDIR_IN;
	redir_in->cmd.args = NULL;
	redir_in->children = NULL;
	redir_in->file = "file";

	t_ast	*redir_out;
	redir_out = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_out)
		exit(1);
	redir_out->type = NODE_REDIR_OUT;
	redir_out->cmd.args = NULL;
	redir_out->children = NULL;
	redir_out->file = "outfile";

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "grep", "foo");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!cmd1->children)
		exit(1);
	cmd1->children[0] = redir_in;
	cmd1->children[1] = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "wc", "-l");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!cmd2->children)
		exit(1);
	cmd2->children[0] = redir_out;
	cmd2->children[1] = NULL;
	cmd2->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = cmd1;
	pipe->children[1] = cmd2;
	pipe->children[2] = NULL;
	pipe->file = NULL;
	
	set_root_node(pipe, pipe);

	return (pipe);
}

t_ast	*make_double_redirs_multi_pipe(void)
{
	// < file grep foo | grep bar | grep foobar | wc -l > outfile

	t_ast	*redir_in;
	redir_in = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_in)
		exit(1);
	redir_in->type = NODE_REDIR_IN;
	redir_in->cmd.args = NULL;
	redir_in->children = NULL;
	redir_in->file = "file";

	t_ast	*redir_out;
	redir_out = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_out)
		exit(1);
	redir_out->type = NODE_REDIR_OUT;
	redir_out->cmd.args = NULL;
	redir_out->children = NULL;
	redir_out->file = "outfile";

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "grep", "foo");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!cmd1->children)
		exit(1);
	cmd1->children[0] = redir_in;
	cmd1->children[1] = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "grep", "bar");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*cmd3;
	cmd3 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd3)
		exit(1);
	cmd3->type = NODE_CMD;
	cmd3->cmd.args = make_args(2, "grep", "foobar");
	cmd3->cmd.fd_in = STDIN_FILENO;
	cmd3->cmd.fd_out = STDOUT_FILENO;
	cmd3->children = NULL;
	cmd3->file = NULL;

	t_ast	*cmd4;
	cmd4 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd4)
		exit(1);
	cmd4->type = NODE_CMD;
	cmd4->cmd.args = make_args(2, "wc", "-l");
	cmd4->cmd.fd_in = STDIN_FILENO;
	cmd4->cmd.fd_out = STDOUT_FILENO;
	cmd4->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!cmd4->children)
		exit(1);
	cmd4->children[0] = redir_out;
	cmd4->children[1] = NULL;
	cmd4->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(5 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = cmd1;
	pipe->children[1] = cmd2;
	pipe->children[2] = cmd3;
	pipe->children[3] = cmd4;
	pipe->children[4] = NULL;
	pipe->file = NULL;

	set_root_node(pipe, pipe);
	
	return (pipe);
}

t_ast	*make_and_if(void)
{
	// cat file && echo ok

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "echo", "ok");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*and;
	and = (t_ast *) malloc(sizeof(t_ast));
	if (!and)
		exit(1);
	and->type = NODE_AND_IF;
	and->cmd.args = NULL;
	and->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and->children)
		exit(1);
	and->children[0] = cmd1;
	and->children[1] = cmd2;
	and->children[2] = NULL;
	and->file = NULL;
	
	set_root_node(and, and);

	return (and);
}

t_ast	*make_or_if(void)
{
	// cat file || echo fail

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "echo", "fail");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*or;
	or = (t_ast *) malloc(sizeof(t_ast));
	if (!or)
		exit(1);
	or->type = NODE_OR_IF;
	or->cmd.args = NULL;
	or->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!or->children)
		exit(1);
	or->children[0] = cmd1;
	or->children[1] = cmd2;
	or->children[2] = NULL;
	or->file = NULL;
	
	set_root_node(or, or);

	return (or);
}

t_ast	*make_and_or_if(void)
{
	// cat file && echo ok || echo fail
	
	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "echo", "ok");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*cmd3;
	cmd3 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd3)
		exit(1);
	cmd3->type = NODE_CMD;
	cmd3->cmd.args = make_args(2, "echo", "fail");
	cmd3->cmd.fd_in = STDIN_FILENO;
	cmd3->cmd.fd_out = STDOUT_FILENO;
	cmd3->children = NULL;
	cmd3->file = NULL;

	t_ast	*and;
	and = (t_ast *) malloc(sizeof(t_ast));
	if (!and)
		exit(1);
	and->type = NODE_AND_IF;
	and->cmd.args = NULL;
	and->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and->children)
		exit(1);
	and->children[0] = cmd1;
	and->children[1] = cmd2;
	and->children[2] = NULL;
	and->file = NULL;
	
	t_ast	*or;
	or = (t_ast *) malloc(sizeof(t_ast));
	if (!or)
		exit(1);
	or->type = NODE_OR_IF;
	or->cmd.args = NULL;
	or->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!or->children)
		exit(1);
	or->children[0] = and;
	or->children[1] = cmd3;
	or->children[2] = NULL;
	or->file = NULL;
	
	set_root_node(or, or);

	return (or);
}

t_ast	*make_demo(void)
{
	// < infile grep foo | wc -l > outfile && echo ok || echo fail

	t_ast	*redir_in;
	redir_in = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_in)
		exit(1);
	redir_in->type = NODE_REDIR_IN;
	redir_in->cmd.args = NULL;
	redir_in->children = NULL;
	redir_in->file = "infile";
	
	t_ast	*redir_out;
	redir_out = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_out)
		exit(1);
	redir_out->type = NODE_REDIR_OUT;
	redir_out->cmd.args = NULL;
	redir_out->children = NULL;
	redir_out->file = "outfile";

	t_ast	*grep;
	grep = (t_ast *) malloc(sizeof(t_ast));
	if (!grep)
		exit(1);
	grep->type = NODE_CMD;
	grep->cmd.args = make_args(2, "grep", "foo");
	grep->cmd.fd_in = STDIN_FILENO;
	grep->cmd.fd_out = STDOUT_FILENO;
	grep->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!grep->children)
		exit(1);
	grep->children[0] = redir_in;
	grep->children[1] = NULL;
	grep->file = NULL;

	t_ast	*wc;
	wc = (t_ast *) malloc(sizeof(t_ast));
	if (!wc)
		exit(1);
	wc->type = NODE_CMD;
	wc->cmd.args = make_args(2, "wc", "-l");
	wc->cmd.fd_in = STDIN_FILENO;
	wc->cmd.fd_out = STDOUT_FILENO;
	wc->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!wc->children)
		exit(1);
	wc->children[0] = redir_out;
	wc->children[1] = NULL;
	wc->file = NULL;

	t_ast	*echo_ok;
	echo_ok = (t_ast *) malloc(sizeof(t_ast));
	if (!echo_ok)
		exit(1);
	echo_ok->type = NODE_CMD;
	echo_ok->cmd.args = make_args(2, "echo", "ok");
	echo_ok->cmd.fd_in = STDIN_FILENO;
	echo_ok->cmd.fd_out = STDOUT_FILENO;
	echo_ok->children = NULL;
	echo_ok->file = NULL;

	t_ast	*echo_fail;
	echo_fail = (t_ast *) malloc(sizeof(t_ast));
	if (!echo_fail)
		exit(1);
	echo_fail->type = NODE_CMD;
	echo_fail->cmd.args = make_args(2, "echo", "fail");
	echo_fail->cmd.fd_in = STDIN_FILENO;
	echo_fail->cmd.fd_out = STDOUT_FILENO;
	echo_fail->children = NULL;
	echo_fail->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = grep;
	pipe->children[1] = wc;
	pipe->children[2] = NULL;

	t_ast	*and;
	and = (t_ast *) malloc(sizeof(t_ast));
	if (!and)
		exit(1);
	and->type = NODE_AND_IF;
	and->cmd.args = NULL;
	and->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and->children)
		exit(1);
	and->children[0] = pipe;
	and->children[1] = echo_ok;
	and->children[2] = NULL;
	and->file = NULL;

	t_ast	*or;
	or = (t_ast *) malloc(sizeof(t_ast));
	if (!or)
		exit(1);
	or->type = NODE_OR_IF;
	or->cmd.args = NULL;
	or->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!or->children)
		exit(1);
	or->children[0] = and;
	or->children[1] = echo_fail;
	or->children[2] = NULL;
	or->file = NULL;

	set_root_node(or, or);

	return (or);
}

t_ast	*make_pipe_and_parentheses(void)
{
	// cat file | (grep foo && grep bar)

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "grep", "foo");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*cmd3;
	cmd3 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd3)
		exit(1);
	cmd3->type = NODE_CMD;
	cmd3->cmd.args = make_args(2, "grep", "bar");
	cmd3->cmd.fd_in = STDIN_FILENO;
	cmd3->cmd.fd_out = STDOUT_FILENO;
	cmd3->children = NULL;
	cmd3->file = NULL;

	t_ast	*and;
	and = (t_ast *) malloc(sizeof(t_ast));
	if (!and)
		exit(1);
	and->type = NODE_AND_IF;
	and->cmd.args = NULL;
	and->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and->children)
		exit(1);
	and->children[0] = cmd2;
	and->children[1] = cmd3;
	and->children[2] = NULL;
	and->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = cmd1;
	pipe->children[1] = and;
	pipe->children[2] = NULL;
	pipe->file = NULL;

	set_root_node(pipe, pipe);

	return (pipe);
}

t_ast	*make_pipe_or_parentheses(void)
{
	// cat file | (grep foo || grep bar)

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "grep", "foo");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*cmd3;
	cmd3 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd3)
		exit(1);
	cmd3->type = NODE_CMD;
	cmd3->cmd.args = make_args(2, "grep", "bar");
	cmd3->cmd.fd_in = STDIN_FILENO;
	cmd3->cmd.fd_out = STDOUT_FILENO;
	cmd3->children = NULL;
	cmd3->file = NULL;

	t_ast	*or;
	or = (t_ast *) malloc(sizeof(t_ast));
	if (!or)
		exit(1);
	or->type = NODE_AND_IF;
	or->cmd.args = NULL;
	or->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!or->children)
		exit(1);
	or->children[0] = cmd2;
	or->children[1] = cmd3;
	or->children[2] = NULL;
	or->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = cmd1;
	pipe->children[1] = or;
	pipe->children[2] = NULL;
	pipe->file = NULL;

	set_root_node(pipe, pipe);

	return (pipe);
}

t_ast	*make_wtfisgoingon(void)
{
	// cat file | (cat infile && grep foo)

	t_ast	*cmd1;
	cmd1 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd1)
		exit(1);
	cmd1->type = NODE_CMD;
	cmd1->cmd.args = make_args(2, "cat", "file");
	cmd1->cmd.fd_in = STDIN_FILENO;
	cmd1->cmd.fd_out = STDOUT_FILENO;
	cmd1->children = NULL;
	cmd1->file = NULL;

	t_ast	*cmd2;
	cmd2 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd2)
		exit(1);
	cmd2->type = NODE_CMD;
	cmd2->cmd.args = make_args(2, "cat", "infile");
	cmd2->cmd.fd_in = STDIN_FILENO;
	cmd2->cmd.fd_out = STDOUT_FILENO;
	cmd2->children = NULL;
	cmd2->file = NULL;

	t_ast	*cmd3;
	cmd3 = (t_ast *) malloc(sizeof(t_ast));
	if (!cmd3)
		exit(1);
	cmd3->type = NODE_CMD;
	cmd3->cmd.args = make_args(2, "grep", "foo");
	cmd3->cmd.fd_in = STDIN_FILENO;
	cmd3->cmd.fd_out = STDOUT_FILENO;
	cmd3->children = NULL;
	cmd3->file = NULL;

	t_ast	*and;
	and = (t_ast *) malloc(sizeof(t_ast));
	if (!and)
		exit(1);
	and->type = NODE_AND_IF;
	and->cmd.args = NULL;
	and->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and->children)
		exit(1);
	and->children[0] = cmd2;
	and->children[1] = cmd3;
	and->children[2] = NULL;
	and->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = cmd1;
	pipe->children[1] = and;
	pipe->children[2] = NULL;
	pipe->file = NULL;

	set_root_node(pipe, pipe);

	return (pipe);

}

t_ast	*make_echo(void)
{
	// echo -n hello

	t_ast	*echo;
	echo = (t_ast *) malloc(sizeof(t_ast));
	if (!echo)
		exit(1);
	echo->type = NODE_CMD;
	echo->cmd.args = make_args(3, "echo", "-n", "hello");
	echo->cmd.fd_in = STDIN_FILENO;
	echo->cmd.fd_out = STDOUT_FILENO;
	echo->children = NULL;
	echo->file = NULL;

	set_root_node(echo, echo);

	return (echo);
}

t_ast	*make_echo_redir(void)
{
	// echo hello there > outfile

	t_ast	*redir_out;
	redir_out = (t_ast *) malloc(sizeof(t_ast));
	if (!redir_out)
		exit(1);
	redir_out->type = NODE_REDIR_OUT;
	redir_out->cmd.args = NULL;
	redir_out->children = NULL;
	redir_out->file = "outfile";

	t_ast	*echo;
	echo = (t_ast *) malloc(sizeof(t_ast));
	if (!echo)
		exit(1);
	echo->type = NODE_CMD;
	echo->cmd.args = make_args(3, "echo", "hello", "there");
	echo->cmd.fd_in = STDIN_FILENO;
	echo->cmd.fd_out = STDOUT_FILENO;
	echo->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!echo->children)
		exit(1);
	echo->children[0] = redir_out;
	echo->children[1] = NULL;
	echo->file = NULL;

	set_root_node(echo, echo);

	return (echo);
}

t_ast	*make_echo_pipe(void)
{
	// echo hello there | wc -w

	t_ast	*echo;
	echo = (t_ast *) malloc(sizeof(t_ast));
	if (!echo)
		exit(1);
	echo->type = NODE_CMD;
	echo->cmd.args = make_args(3, "echo", "hello", "there");
	echo->cmd.fd_in = STDIN_FILENO;
	echo->cmd.fd_out = STDOUT_FILENO;
	echo->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!echo->children)
		exit(1);
	echo->children = NULL;
	echo->file = NULL;

	t_ast	*wc;
	wc = (t_ast *) malloc(sizeof(t_ast));
	if (!wc)
		exit(1);
	wc->type = NODE_CMD;
	wc->cmd.args = make_args(2, "wc", "-w");
	wc->cmd.fd_in = STDIN_FILENO;
	wc->cmd.fd_out = STDOUT_FILENO;
	wc->children = NULL;
	wc->file = NULL;

	t_ast	*pipe;
	pipe = (t_ast *) malloc(sizeof(t_ast));
	if (!pipe)
		exit(1);
	pipe->type = NODE_PIPE;
	pipe->cmd.args = NULL;
	pipe->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!pipe->children)
		exit(1);
	pipe->children[0] = echo;
	pipe->children[1] = wc;
	pipe->children[2] = NULL;

	set_root_node(pipe, pipe);

	return (pipe);
}

t_ast	*make_cd_pwd(void)
{
	// cd srcs && pwd || echo fail

	t_ast	*cd;
	cd = (t_ast *) malloc(sizeof(t_ast));
	if (!cd)
		exit(1);
	cd->type = NODE_CMD;
	cd->cmd.args = make_args(2, "cd", "srcs");
	cd->cmd.fd_in = STDIN_FILENO;
	cd->cmd.fd_out = STDOUT_FILENO;
	cd->children = NULL;
	cd->file = NULL;

	t_ast	*pwd;
	pwd = (t_ast *) malloc(sizeof(t_ast));
	if (!pwd)
		exit(1);
	pwd->type = NODE_CMD;
	pwd->cmd.args = make_args(1, "pwd");
	pwd->cmd.fd_in = STDIN_FILENO;
	pwd->cmd.fd_out = STDOUT_FILENO;
	pwd->children = NULL;
	pwd->file = NULL;

	t_ast	*echo;
	echo = (t_ast *) malloc(sizeof(t_ast));
	if (!echo)
		exit(1);
	echo->type = NODE_CMD;
	echo->cmd.args = make_args(2, "echo", "fail");
	echo->cmd.fd_in = STDIN_FILENO;
	echo->cmd.fd_out = STDOUT_FILENO;
	echo->children = NULL;
	echo->file = NULL;

	t_ast	*and;
	and = (t_ast *) malloc(sizeof(t_ast));
	if (!and)
		exit(1);
	and->type = NODE_AND_IF;
	and->cmd.args = NULL;
	and->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and->children)
		exit(1);
	and->children[0] = cd;
	and->children[1] = pwd;
	and->children[2] = NULL;
	and->file = NULL;

	t_ast	*or;
	or = (t_ast *) malloc(sizeof(t_ast));
	if (!or)
		exit(1);
	or->type = NODE_OR_IF;
	or->cmd.args = NULL;
	or->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!or->children)
		exit(1);
	or->children[0] = and;
	or->children[1] = echo;
	or->children[2] = NULL;
	or->file = NULL;

	set_root_node(or, or);
	
	return (or);
}

t_ast	*make_env(void)
{
	// env

	t_ast	*env;
	env = (t_ast *) malloc(sizeof(t_ast));
	if (!env)
		exit(1);
	env->type = NODE_CMD;
	env->cmd.args = make_args(1, "env");
	env->cmd.fd_in = STDIN_FILENO;
	env->cmd.fd_out = STDOUT_FILENO;
	env->children = NULL;
	env->file = NULL;

	set_root_node(env, env);

	return (env);
}

t_ast	*make_heredoc_ast(void)
{
	// << end grep foo
	t_ast	*here;
	here = (t_ast *) malloc(sizeof(t_ast));
	if (!here)
		exit(1);
	here->type = NODE_HEREDOC;
	here->cmd.args = NULL;
	here->children = NULL;
	here->file = "end";

	t_ast	*grep;
	grep = (t_ast *) malloc(sizeof(t_ast));
	if (!grep)
		exit(1);
	grep->type = NODE_CMD;
	grep->cmd.args = make_args(2, "grep", "foo");
	grep->cmd.fd_in = STDIN_FILENO;
	grep->cmd.fd_out = STDOUT_FILENO;
	grep->children = (t_ast **) malloc(2 * sizeof(t_ast *));
	if (!grep->children)
		exit(1);
	grep->children[0] = here;
	grep->children[1] = NULL;
	grep->file = NULL;

	set_root_node(grep, grep);
	return (grep);
}

t_ast	*make_sleep(void)
{
	t_ast	*sleep;
	sleep = (t_ast *) malloc(sizeof(t_ast));
	if (!sleep)
		exit(1);
	sleep->type = NODE_CMD;
	sleep->cmd.args = make_args(2, "sleep", "3");
	sleep->cmd.fd_in = STDIN_FILENO;
	sleep->cmd.fd_out = STDOUT_FILENO;
	sleep->children = NULL;
	sleep->file = NULL;

	set_root_node(sleep, sleep);
	return (sleep);
}

t_ast	*make_export(void)
{
	// export foo=42 && env && export foo=13 && env && unset foo && env

	t_ast	*export1;
	export1 = (t_ast *) malloc(sizeof(t_ast));
	if (!export1)
		exit(1);
	export1->type = NODE_CMD;
	export1->cmd.args = make_args(2, "export", "foo=42");
	export1->cmd.fd_in = STDIN_FILENO;
	export1->cmd.fd_out = STDOUT_FILENO;
	export1->children = NULL;
	export1->file = NULL;

	t_ast	*export2;
	export2 = (t_ast *) malloc(sizeof(t_ast));
	if (!export2)
		exit(1);
	export2->type = NODE_CMD;
	export2->cmd.args = make_args(2, "export", "foo=13");
	export2->cmd.fd_in = STDIN_FILENO;
	export2->cmd.fd_out = STDOUT_FILENO;
	export2->children = NULL;
	export2->file = NULL;

	t_ast	*env1;
	env1 = (t_ast *) malloc(sizeof(t_ast));
	if (!env1)
		exit(1);
	env1->type = NODE_CMD;
	env1->cmd.args = make_args(1, "env");
	env1->cmd.fd_in = STDIN_FILENO;
	env1->cmd.fd_out = STDOUT_FILENO;
	env1->children = NULL;
	env1->file = NULL;

	t_ast	*env2;
	env2 = (t_ast *) malloc(sizeof(t_ast));
	if (!env2)
		exit(1);
	env2->type = NODE_CMD;
	env2->cmd.args = make_args(1, "env");
	env2->cmd.fd_in = STDIN_FILENO;
	env2->cmd.fd_out = STDOUT_FILENO;
	env2->children = NULL;
	env2->file = NULL;

	t_ast	*env3;
	env3 = (t_ast *) malloc(sizeof(t_ast));
	if (!env3)
		exit(1);
	env3->type = NODE_CMD;
	env3->cmd.args = make_args(1, "env");
	env3->cmd.fd_in = STDIN_FILENO;
	env3->cmd.fd_out = STDOUT_FILENO;
	env3->children = NULL;
	env3->file = NULL;

	t_ast	*unset;
	unset = (t_ast *) malloc(sizeof(t_ast));
	if (!unset)
		exit(1);
	unset->type = NODE_CMD;
	unset->cmd.args = make_args(2, "unset", "foo");
	unset->cmd.fd_in = STDIN_FILENO;
	unset->cmd.fd_out = STDOUT_FILENO;
	unset->children = NULL;
	unset->file = NULL;

	t_ast	*and1;
	and1 = (t_ast *) malloc(sizeof(t_ast));
	if (!and1)
		exit(1);
	and1->type = NODE_AND_IF;
	and1->cmd.args = NULL;
	and1->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and1->children)
		exit(1);
	and1->children[0] = export1;
	and1->children[1] = env1;
	and1->children[2] = NULL;
	and1->file = NULL;

	t_ast	*and2;
	and2 = (t_ast *) malloc(sizeof(t_ast));
	if (!and2)
		exit(1);
	and2->type = NODE_AND_IF;
	and2->cmd.args = NULL;
	and2->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and2->children)
		exit(1);
	and2->children[0] = and1;
	and2->children[1] = export2;
	and2->children[2] = NULL;
	and2->file = NULL;

	t_ast	*and3;
	and3 = (t_ast *) malloc(sizeof(t_ast));
	if (!and3)
		exit(1);
	and3->type = NODE_AND_IF;
	and3->cmd.args = NULL;
	and3->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and3->children)
		exit(1);
	and3->children[0] = and2;
	and3->children[1] = env2;
	and3->children[2] = NULL;
	and3->file = NULL;

	t_ast	*and4;
	and4 = (t_ast *) malloc(sizeof(t_ast));
	if (!and4)
		exit(1);
	and4->type = NODE_AND_IF;
	and4->cmd.args = NULL;
	and4->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and4->children)
		exit(1);
	and4->children[0] = and3;
	and4->children[1] = unset;
	and4->children[2] = NULL;
	and4->file = NULL;

	t_ast	*and5;
	and5 = (t_ast *) malloc(sizeof(t_ast));
	if (!and5)
		exit(1);
	and5->type = NODE_AND_IF;
	and5->cmd.args = NULL;
	and5->children = (t_ast **) malloc(3 * sizeof(t_ast *));
	if (!and5->children)
		exit(1);
	and5->children[0] = and4;
	and5->children[1] = env3;
	and5->children[2] = NULL;
	and5->file = NULL;

	set_root_node(and5, and5);
	return (and5);
}

t_ast	*make_ast(int mode)
{
	if (mode == 0)
		return (make_simple_pipe());
	else if (mode == 1)
		return (make_multi_pipe());
	else if (mode == 2)
		return (make_double_redirs());
	else if (mode == 3)
		return (make_double_redirs_simple_pipe());
	else if (mode == 4)
		return (make_double_redirs_multi_pipe());
	else if (mode == 5)
		return (make_and_if());
	else if (mode == 6)
		return (make_or_if());
	else if (mode == 7)
		return (make_and_or_if());
	else if (mode == 8)
		return (make_demo());
	else if (mode == 9)
		return (make_pipe_and_parentheses());
	else if (mode == 10)
		return (make_pipe_or_parentheses());
	else if (mode == 11)
		return (make_wtfisgoingon());
	else if (mode == 12)
		return (make_echo());
	else if (mode == 13)
		return (make_echo_redir());
	else if (mode == 14)
		return (make_echo_pipe());
	else if (mode == 15)
		return (make_cd_pwd());
	else if (mode == 16)
		return (make_env());
	else if (mode == 17)
		return (make_heredoc_ast());
	else if (mode == 18)
		return (make_sleep());
	else if (mode == 19)
		  return (make_export());
	return (NULL);
}

