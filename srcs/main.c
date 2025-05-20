/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 23:47:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/05/13 19:52:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* Boucle infinie où pour chaque itération,
 * 1)	Réception de la commande et affichage du prompt.
 * 		1.1)	cwd (current working directory) est initialisé avec make_cwd().
 * 		1.2)	command est récupérée avec readline(), et cwd est transmis en
 * 				paramètre. cwd est affichée comme invite de commande par
 * 				readline().
 * 		1.3)	cwd est libéré.
 *
 * 2)	Si une commande est reçue (peut être seulement un charactère NULL en cas
 * 		de commande vide transmise par l'utilisateur).
 * 		2.1)	preset est initialisé avec ft_atoi().
 * 				Pour l'instant, le programme ne cherche qu'à exécuter des tests
 * 				pré-parsés manuellement et rassemblés dans une liste de presets,
 * 				la seule commande que le prompt peut recevoir est le chiffre
 * 				correspondant à un preset existant, soit un int récupéré par
 * 				ft_atoi().
 * 		2.2)	command est ajoutée à l'historique.
 * 		2.3) Si...
 * 			2.3.0)	Si le preset est inférieur au nombre max de presets ou que son
 * 					premier caractère n'est pas un nombre, imprime les options et
 * 					libère command.
 * 			 Sinon...
 * 			2.3.1)	Sinon, command est libérée et la fonction print_cmd() est appelée
 * 					pour informer l'utilisateur de la commande émulée par le test
 * 					en cours.
 * 			2.3.2)	ast est initialisé avec les presets hardcodés en appeland
 * 					make_ast(), puis la référence dans la node racine à paths est
 * 					initialisée avec get_paths(), qui récupère tous les paths
 * 					présents dans l'environnement. Finalement, le pointeur
 * 					d'environnement est copié dans la racine de l'ast.
 * 			2.3.3)	exec_ast() exécute la simulation d'AST qui lui est transmise.
 * 					La racine récupérée par le parsing de minishell devra lui
 * 					être transmise comme argument pour l'exécution.
 * 			2.3.4)	cleanup() est finalement appelé pour libérer toute mémoire
 * 					allouée.
 * 3)	Si aucune commande n'est reçue, la boucle est brisée et le programme se termine.
*/

int main(int ac, char **av, char **envp)
{
	t_ast	*ast;
	char	*command;
	char	*cwd;
	int		preset;

	(void) ac;
	(void) av;
	
	while (1)
	{
		cwd = make_cwd();
		command = readline(cwd);
		free(cwd);
		if (command)
		{
			preset = ft_atoi(command);
			add_history(command);
			if (preset > 18 || !ft_isdigit(command[0]))
			{
				print_options();
				free(command);
			}
			else
			{
				free(command);
				print_cmd(preset);
				ast = make_ast(preset);
				ast->paths = get_paths();
				ast->envp = envp;
				exec_ast(ast);
				cleanup(ast->root);
			}
		}
		else
			break;
	}
	return (0);
}

/*
int main(int ac, char **av, char **envp)
{
	t_ast	*ast;
	char	**paths;
	int		mode;

	if (ac != 2)
		return (1);
	mode = ft_atoi(av[1]);
	if (mode > 188888888)
	{
		printf("Invalid preset\n");
		return (1);
	}
	print_cmd(mode);
	paths = get_paths();
	ast = make_ast(mode);
	ast->paths = paths;
	ast->envp = envp;
//	print_tree(ast);
	exec_ast(ast);
	cleanup(ast);
	return (0);
}
*/
