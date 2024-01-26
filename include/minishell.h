/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:35:26 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/26 01:59:16 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H 1

/* TODO: remove debug include */
# include "minishell_debug.h"
# define PS0 "> "
# define PS1 "\\u@\\h:\\w\\$ "

# define ERR_NEW_TOKEN      "Error while creating new token."

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
	TOK_SQOUTE_STR,
	TOK_DQOUTE_STR,
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
