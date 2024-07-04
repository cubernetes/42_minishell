/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:28:14 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 17:30:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	_initalize_tree_ht(t_ht *ht[MAX_HT_SIZE + 1])
{
	ht_set(ht, "<pipe_sequence>", as_tree_type(PIPE_SEQUENCE));
	ht_set(ht, "<complete_command_tail>", as_tree_type(COMPLETE_COMMAND_TAIL));
	ht_set(ht, "<and_or>", as_tree_type(AND_OR));
	ht_set(ht, "<command>", as_tree_type(COMMAND));
	ht_set(ht, "<pipe_sequence_tail>", as_tree_type(PIPE_SEQUENCE_TAIL));
	ht_set(ht, "<simple_command>", as_tree_type(SIMPLE_COMMAND));
	ht_set(ht, "<compound_command>", as_tree_type(COMPOUND_COMMAND));
	ht_set(ht, "<complete_command>", as_tree_type(COMPLETE_COMMAND));
	ht_set(ht, "<io_redirect>", as_tree_type(IO_REDIRECT));
	ht_set(ht, "<simple_command_tail>", as_tree_type(SIMPLE_COMMAND_TAIL));
	ht_set(ht, "(", as_tree_type(TOKEN));
	ht_set(ht, ")", as_tree_type(TOKEN));
	ht_set(ht, ">", as_tree_type(TOKEN));
	ht_set(ht, "<", as_tree_type(TOKEN));
	ht_set(ht, "||", as_tree_type(TOKEN));
	ht_set(ht, "&&", as_tree_type(TOKEN));
	ht_set(ht, ">>", as_tree_type(TOKEN));
	ht_set(ht, "<<", as_tree_type(TOKEN));
	ht_set(ht, "TOK_WORD", as_tree_type(TOKEN));
	ht_set(ht, "TOK_EPSILON", as_tree_type(TOKEN));
	ht_set(ht, "|", as_tree_type(TOKEN));
}

t_tree_type	tree_ht_get(char *key)
{
	static t_ht	*ht[MAX_HT_SIZE + 1];

	if (ht[MAX_HT_SIZE] != NULL)
		return (ht_get(ht, key).as_tree_type);
	gc_start_context("POST");
	ft_bzero(ht, sizeof(*ht) * (MAX_HT_SIZE + 1));
	_initalize_tree_ht(ht);
	ht[MAX_HT_SIZE] = (void *)1;
	gc_end_context();
	return (ht_get(ht, key).as_tree_type);
}

t_token_type	tokens_ht_get(char *key)
{
	static t_ht	*ht[MAX_HT_SIZE + 1];

	if (ht[MAX_HT_SIZE] == NULL)
	{
		gc_start_context("POST");
		ft_bzero(ht, sizeof(*ht) * (MAX_HT_SIZE + 1));
		ht_set(ht, "TOK_EPSILON", as_token_type(TOK_EPSILON));
		ht_set(ht, "TOK_WORD", as_token_type(TOK_WORD));
		ht_set(ht, "<<", as_token_type(TOK_HEREDOC));
		ht_set(ht, ">>", as_token_type(TOK_APPEND));
		ht_set(ht, "2>>", as_token_type(TOK_APPEND_ERR));
		ht_set(ht, "<", as_token_type(TOK_INPUT));
		ht_set(ht, ">", as_token_type(TOK_OVERRIDE));
		ht_set(ht, "2>", as_token_type(TOK_OVERRIDE_ERR));
		ht_set(ht, "(", as_token_type(TOK_L_PAREN));
		ht_set(ht, ")", as_token_type(TOK_R_PAREN));
		ht_set(ht, "&&", as_token_type(TOK_AND));
		ht_set(ht, "||", as_token_type(TOK_OR));
		ht_set(ht, ";", as_token_type(TOK_SEMI));
		ht_set(ht, "|", as_token_type(TOK_PIPE));
		ht[MAX_HT_SIZE] = (void *)1;
		gc_end_context();
	}
	return (ht_get(ht, key).as_token_type);
}
