/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_production_idx.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:43:59 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 17:54:23 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_token_type	adjust_type(t_token_type type)
{
	if (type == TOK_DQUOTE_STR || type == TOK_SQUOTE_STR)
		type = TOK_WORD;
	else if (type == TOK_SEMI)
		type = TOK_AND;
	else if (type == TOK_OVERRIDE_ERR)
		type = TOK_OVERRIDE;
	else if (type == TOK_APPEND_ERR)
		type = TOK_APPEND;
	return (type);
}

void	handle_syntax_error(t_token *token)
{
	if (!*token->str)
		set_last_exit_status(minishell_error(2, !option_enabled('i'), true,
				"syntax error near unexpected token `newline'"));
	else
		set_last_exit_status(minishell_error(2, !option_enabled('i'), true,
				"syntax error near unexpected token `%s'",
				token->str));
}

/* return the index of the production to use */
/* TODO: fix error handling */
/* TODO: Not required: Make transition table adaptive (quite a lot of work) */
int	get_production_idx(t_tree_type nonterm, t_token *token)
{
	static int		transition_table[NUM_NONTERMS][NUM_TOKENS] = {
	{19, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0},
	{1, 2, 2, -1, -1, 1, -1, -1, -1, -1, -1},
	{-1, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, 5, -1, 5, 5, 5, 5, 5},
	{6, 6, 6, 7, -1, 6, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, 9, -1, 8, 8, 8, 8, 8},
	{-1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, 11, 11, 11, 11, 11},
	{12, 12, 12, 12, -1, 12, 13, 13, 13, 13, 13},
	{-1, -1, -1, -1, -1, -1, 14, 17, 15, 18, 16}
	};
	int				production_idx;
	t_token_type	type;

	type = adjust_type(token->type);
	production_idx = transition_table[nonterm - 1][type - 1];
	if (production_idx == -1)
		handle_syntax_error(token);
	return (production_idx);
}
