/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitran <pitran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:21:16 by imeulema          #+#    #+#             */
/*   Updated: 2025/06/10 17:36:23 by pitran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H


/* Enums */
typedef enum e_token_type
{
	DEFAULT = 0,
	WORD = 1,
	ASSIGNMENT = 2,
	QUOTE = 3,
	SINGLE_QUOTE = 4,
	DOUBLE_QUOTE = 5,
	REDIR = 6,
	REDIR_IN = 7,
	REDIR_OUT = 8,
	REDIR_APPEND = 9,
	HEREDOC = 10,
	OPERATOR = 11,
	PIPE = 12,
	AND = 13,
	OR = 14,
	SEMICOLON = 15,
	PARENTHESIS = 16,
	PAREN_OPEN = 17,
	PAREN_CLOSE = 18,
	SPECIAL_CARACTER = 19,
	ENV_VAR = 20,
	EXIT_STATUS = 21,
	ESCAPE = 22,
	COMMENT = 23,
	T_NEWLINE = 24,
	T_EOF = 25,
	UNKNOWN = 26
}	t_token_type;

/* Structs */
typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;


/* Functions */
/* Parsing functions */
t_node_type	token_to_node_type(t_token_type type);

t_token		**extract_token(char **command, t_token_type type, t_token **token_list);
t_token		**tokenize_input(char *command);
t_token		*create_token(void);
t_token		*get_token_at_index(t_token **token_list, int index);
t_token		*handle_token_type(char **command, t_token_type type, t_token *new_token);
t_token		*tokenize_operator(char **command, t_token *token);
t_token		*tokenize_parenthesis(char **command, t_token *token);
t_token		*tokenize_quote(char **command, t_token *token);
t_token		*tokenize_redir(char **command, t_token *token);
t_token		*tokenize_special_caracter(char **command, t_token *token);
t_token		*tokenize_word(char **command, t_token *token);

t_ast		**extract_redirections(t_token **tokens, int start, int end, t_ast *root);
t_ast		*create_ast(t_token **token_list);
t_ast		*create_ast_node(t_node_type type, t_ast *root);
t_ast		*create_command_node(char **args, t_ast *root);
t_ast		*create_operator_node(t_node_type type, t_ast *left, t_ast *right, t_ast *root);
t_ast		*create_redirection_node(t_node_type type, char *file, t_ast *root);
t_ast		*create_subshell_node(t_ast *child, t_ast *root);
t_ast		*parse_command_line(t_token **tokens, int start, int end, t_ast *root);
t_ast 		*parse_input(char *command, t_shell_data *shell);
t_ast		*parse_simple_command(t_token **tokens, int start, int end, t_ast *root);

char		**extract_args(t_token **tokens, int start, int end);
//	char		*ft_strdup(const char *s);

//	void		*ft_memset(void *s, int c, size_t n);
void		add_child_to_node(t_ast *parent, t_ast *child);
void		define_bonus_operator(char **command, t_token *token, int *len);
void		define_double_redirection(char **command, t_token *token, int *len);
void		define_special_caracter(char **command, t_token *token);
void		free_ast(t_ast *node);
void		free_string_array(char **array);
void		free_token(t_token *token);
void		free_token_list(t_token **token_list);
void		link_token(t_token *token, t_token **token_list);
void		print_ast(t_ast *node, int depth);
void		print_token_list(t_token **token_list);

int			check_token(t_token *cur, t_token **stack, int *top);
int			count_args(t_token **tokens, int start, int end);
int			count_redirections(t_token **tokens, int start, int end);
int			count_tokens(t_token **token_list);
int			find_lowest_precedence_op(t_token **tokens, int start, int end);
int			find_matching_parenthesis(t_token **tokens, int open_pos, int end);
int			find_word_len(char *content);
int			get_operator_precedence(t_token_type type);
int			is_command_char(char c);
int			is_in_subshell(t_token **token_list, int index);
int			is_operator(char c);
int			is_parenthesis(char c);
int			is_quote(char c);
int			is_redirection(char c);
int			is_redirection_token(t_token_type type);
int			is_space(char c);
int			is_special_caracter(char c);
int			matching_parentheses(t_token **tokens, int start, int end);
int			operator_syntax_is_valid(t_token **token_list);
int			paren_syntax_is_valid(t_token **token_list);
int			quote_syntax_is_valid(t_token **token_list);
int			redir_syntax_is_valid(t_token **token_list);
int			syntax_is_valid(t_token **token_list);
void 		set_root_node(t_ast *node, t_ast *root);
char 		*get_str(const char *name);

#endif
