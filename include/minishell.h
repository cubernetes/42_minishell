/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:35:26 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/26 19:44:54 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H 1

/* TODO: remove debug include */
# include "minishell_debug.h"
# define PS0 "> "
/* TODO: use advanced prompt */
/* # define PS1 "\\u@\\h:\\w\\$ " */
# define PS1 "$ "

# define ERR_NEW_TOKEN      "Error while creating new token."

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
	t_token_type	token_type;
	char			*token;
}	t_token;

typedef enum e_bool
{
	FALSE = 0,
	TRUE
}	t_bool;

typedef struct s_ast	t_ast;

struct s_ast
{
	t_token	*token;
	t_bool	is_terminal_tok;
	t_ast	**ast_children;
};

#endif /* minishell.h. */
