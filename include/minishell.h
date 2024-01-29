/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:35:26 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/29 06:24:34 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H 1

/* TODO: remove debug include */
# include "../libft/libft.h"
# include "minishell_debug.h"
# define PS0 "> "
/* TODO: use advanced prompt */
/* # define PS1 "\\u@\\h:\\w\\$ " */
# define PS1 "$ "

# define STR_TOK_OVERRIDE   "TOK_OVERRIDE"
# define STR_TOK_INPUT      "TOK_INPUT"
# define STR_TOK_APPEND     "TOK_APPEND"
# define STR_TOK_HEREDOC    "TOK_HEREDOC"
# define STR_TOK_PIPE       "TOK_PIPE"
# define STR_TOK_AND        "TOK_AND"
# define STR_TOK_OR         "TOK_OR"
# define STR_TOK_L_PAREN    "TOK_L_PAREN"
# define STR_TOK_R_PAREN    "TOK_R_PAREN"
# define STR_TOK_SQUOTE_STR "TOK_SQUOTE_STR"
# define STR_TOK_DQUOTE_STR "TOK_DQUOTE_STR"
# define STR_TOK_WORD       "TOK_WORD"
# define STR_TOK_EOL        "TOK_EOL"
# define STR_TOK_ERROR      "TOK_ERROR"
# define STR_TOK_UNKNOWN    "TOK_UNKNOWN"

# define STR_COMPLETE_COMMAND      "COMPLETE_COMMAND"
# define STR_PIPE_SEQUENCE         "PIPE_SEQUENCE"
# define STR_COMMAND               "COMMAND"
# define STR_COMPOUND_COMMAND      "COMPOUND_COMMAND"
# define STR_SIMPLE_COMMAND        "SIMPLE_COMMAND"
# define STR_IO_REDIRECT           "IO_REDIRECT"
# define STR_TOKEN                 "TOKEN"
# define STR_AST_NODE_TYPE_UNKNOWN "AST_NODE_TYPE_UNKNOWN"

typedef struct s_state
{
	char	*ps0;
	char	*ps1;
}			t_state;

typedef enum e_token_type
{
	TOK_OVERRIDE,
	TOK_INPUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_L_PAREN,
	TOK_R_PAREN,
	TOK_SQUOTE_STR,
	TOK_DQUOTE_STR,
	TOK_WORD,
	TOK_EOL,
	TOK_ERROR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*str;
}	t_token;

typedef struct s_tokens
{
	t_ddeque		*tokens;
	t_ddeque_node	*orig_head;
	t_token			*err_token;
}	t_tokens;

typedef struct s_ast_node	t_ast_node;
typedef t_ast_node			*t_children[];

/* children is a NULL terminated array of t_ast_node ptrs */
typedef union u_ast_node_data
{
	t_ast_node	**children;
	t_token		*token;
}	t_ast_node_data;

/* TOKEN is a TERMINAL, every other member is a NONTERMINAL */
typedef enum e_ast_node_type
{
	COMPLETE_COMMAND,
	PIPE_SEQUENCE,
	COMMAND,
	COMPOUND_COMMAND,
	SIMPLE_COMMAND,
	IO_REDIRECT,
	TOKEN
}	t_ast_node_type;

/* if t_ast_node.type == TOKEN, then t_ast_node.data.token shall be used */
/* if t_ast_node.type != TOKEN, then t_ast_node.data.children
 * shall be used */
struct s_ast_node
{
	t_ast_node_type	type;
	t_ast_node_data	data;
};

#endif /* minishell.h. */
