/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:35:00 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 19:53:13 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// FICHIER D'UTILS AU DEBUGGING

void	print_options(void)
{
	printf("Invalid preset, please enter a number between 0 and 16 (included) to simulate the following presets:\n");
	printf("0:\tcat file | grep foo\n");
	printf("1:\tcat file | grep foo | grep bar | wc -l\n");
	printf("2:\t< file wc -l > outfile\n");
	printf("3:\t< file grep foo | wc -l > outfile\n");
	printf("4:\t< file grep foo | grep bar | grep foobar | wc -l > outfile\n");
	printf("5:\tcat file && echo ok\n");
	printf("6:\tcat file || echo fail\n");
	printf("7:\tcat file && echo ok || echo fail\n");
	printf("8:\t< infile grep foo | wc -l > outfile && echo ok || echo fail\n");
	printf("9:\tcat file | (grep foo && grep bar)\n");
	printf("10:\tcat file | (grep foo || grep bar)\n");
	printf("11:\tcat file | (cat infile && grep foo)\n");
	printf("12\techo -n hello\n");
	printf("13\techo hello there > outfile\n");
	printf("14:\techo hello there | wc -w\n");
	printf("15:\tcd srcs && pwd || echo fail\n");
	printf("16:\tenv\n");
	printf("17:\t[subshell]\n");
	printf("18:\t<< end grep foo\n");

}

void	print_cmd(int mode)
{
	printf("Emulating command:\n");
	if (mode == 0)
		printf("\tcat file | grep foo\n");
	else if (mode == 1)
		printf("\tcat file | grep foo | grep bar | wc -l\n");
	else if (mode == 2)
		printf("\t< file wc -l > outfile\n");
	else if (mode == 3)
		printf("\t< file grep foo | wc -l > outfile\n");
	else if (mode == 4)
		printf("\t< file grep foo | grep bar | grep foobar | wc -l > outfile\n");
	else if (mode == 5)
		printf("\tcat file && echo ok\n");
	else if (mode == 6)
		printf("\tcat file || echo fail\n");
	else if (mode == 7)
		printf("\tcat file && echo ok || echo fail\n");
	else if (mode == 8)
		printf("< infile grep foo | wc -l > outfile && echo ok || echo fail\n");
	else if (mode == 9)
		printf("cat file | (grep foo && grep bar)\n");
	else if (mode == 10)
		printf("cat file | (grep foo || grep bar)\n");
	else if (mode == 11)
		printf("cat file | (cat infile && grep foo)\n");
	else if (mode == 12)
		printf("echo -n hello\n");
	else if (mode == 13)
		printf("echo hello there > outfile\n");
	else if (mode == 14)
		printf("echo hello there | wc -w\n");
	else if (mode == 15)
		printf("cd srcs && pwd || echo fail\n");
	else if (mode == 16)
		printf("env\n");
	else if (mode == 17)
		printf("[subshell]\n");
	else if (mode == 18)
		printf("<< end grep foo\n");
	printf("\n");
}

void	print_node_type(t_ast node)
{
	int type;

	type = node.type;
	if (type == 0)
		printf("Command node\n");
	else if (type == 1)
		printf("Pipe node\n");
	else if (type == 2)
		printf("And-if node\n");
	else if (type == 3)
		printf("Or-if node\n");
	else if (type == 4)
		printf("Redir in node\n");
	else if (type == 5)
		printf("Redir out node\n");
	else if (type == 6)
		printf("Redir append node\n");
	else if (type == 7)
		printf("Heredoc node\n");

}

// imprime les enfants de la node actuelle
void	print_child(t_ast node)
{
	print_node_type(node);
	if (node.type == NODE_CMD)
		printf("%s\n", node.cmd.args[0]);
}

// imprime chaque information pertinente pour la node actuelle
void	print_node_info(t_ast *node)
{
	int	i;

	print_node_type(*node);
	printf("Pointer address: %p\n", node);
	if (node->root)
	{
		printf("Root: ");
		print_node_type(*node->root);
	}
	if (node->cmd.path)
		printf("Path:\n\t%s\n", node->cmd.path);
	if (node->cmd.args)
	{
		printf("Arguments:\n");
		i = -1;
		while (node->cmd.args[++i])
			printf("\t%s\n", node->cmd.args[i]);
	}
	if (node->type == NODE_CMD)
		printf("fd_in = %d\nfd_out = %d\n", node->cmd.fd_in, node->cmd.fd_out);
	if (node->file)
		printf("File: %s\n", node->file);
	if (node->children)
	{
		printf("Children:\n");
		i = -1;
		while (node->children[++i])
			print_child(*(node->children[i]));
	}
	printf("\n");
}

// fonction récursive qui permet d'imprimer la totalité de l'arbre à partir de sa racine
void	print_tree(t_ast *ast)
{
	int	i;

	print_node_info(ast);
	if (ast->children)
	{
		i = -1;
		while (ast->children[++i])
			print_tree(ast->children[i]);
	}
}
