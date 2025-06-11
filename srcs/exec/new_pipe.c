#include "../../incl/minishell.h"

void	link_write_end(t_ast *node, int fd[2][2], int i)
{
	int	j;

	if (node->type == NODE_CMD)
		node->cmd.fd_out = fd[i % 2][1];
	else if (node->children)
	{
		j = -1;
		while (node->children[++j])
		{
			if (node->children[j]->type != NODE_PIPE)
				link_write_end(node->children[j], fd, i);
		}
	}
}

void	link_read_end(t_ast *node, int fd[2][2], int i)
{
	int	j;

	if (node->type == NODE_CMD)
		node->cmd.fd_in = fd[i % 2][0];
	else if (node->children)
	{
		j = -1;
		while (node->children[++j])
		{
			if (node->children[j]->type != NODE_PIPE)
				link_read_end(node->children[j], fd, i);
		}
	}
}

void	new_link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i)
{
	link_write_end(cmd1, fd, i);
	link_read_end(cmd2, fd, i);
}

void	new_exec_pipe_cmd(t_ast *node, int *pids, int i)
{
	printf("call to new_exec_pipe_cmd() made for %s\n", node->cmd.args[0]);
	if (make_redirs(node) == FAILURE)
		pids[i] = -2;
	printf("About to fork in new_exec_pipe_cmd for %s\n", node->cmd.args[0]);
	pids[i] = make_fork();
	if (pids[i] < 0)	// wtf ?
		pids[i] = -2;
	else if (pids[i] == 0)
	{
//		exit(1);
		dup_fds(*node);
		printf("In child process, dup2'd fds for %s\n", node->cmd.args[0]);
		exec_cmd(node, node->cmd);
		printf("uh oh something went bad\n");
		close_redirs(node->cmd);
		clean_exit(node->root, FAILURE);
	}
	close_redirs(node->cmd);
	printf("In parent process, redirs closed for %s\n", node->cmd.args[0]);
	unlink_heredoc(node);
}

void	new_exec_pipe_child(t_ast *node, int *pids, int i)
{
	int	status;

	printf("call to new_exec_pipe_child() made\n");
	status = FAILURE;
	pids[i] = make_fork();
	if (pids[i] == 0)
		status = exec_ast(node);
	if (status == SUCCESS)
		pids[i] = -1;
	else
		pids[i] = -2;
}

int	run_pipe(t_ast **child, int *pids, int count)
{
	int	fd[2][2];
	int	i;

	i = -1;
	while (++i < count)
	{
		if (i + 1 < count)
		{
			if (make_pipe(fd[i % 2]))
				new_link_pipe(child[i], child[i + 1], fd, i);
			else
				return (pipe_error(pids, fd, i, count));
		}
		if (child[i]->type == NODE_CMD && is_builtin(child[i]->cmd))
			exec_pipe_builtin(child[i], pids, i);
		else if (child[i]->type == NODE_CMD)
			new_exec_pipe_cmd(child[i], pids, i);
		else
			new_exec_pipe_child(child[i], pids, i);
		close_pipes(fd, i, count);
	}
	printf("About to call waitpids, i = %d\n", i);
	return (waitpids(pids, count));
}

int	exec_pipe(t_ast **children)
{
	int	*pids;
	int	status;
	int	count;

	count = count_nodes(children);
	pids = init_pids(count);
	if (!pids)
		return (FAILURE);
	status = run_pipe(children, pids, count);
	free(pids);
	return (status);
}
