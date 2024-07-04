/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:15:02 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 19:23:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	_join_word_token(t_token *word_token[static 1], t_token *token)
{
	if (*word_token == NULL)
	{
		*word_token = new_token("", TOK_WORD, true);
		(*word_token)->num_tokens_after_split = 0;
	}
	(*word_token)->num_tokens_after_split
		= ft_max((*word_token)->num_tokens_after_split,
			token->num_tokens_after_split);
	(*word_token)->str = ft_strjoin((*word_token)->str, token->str);
	(*word_token)->origin = ft_strjoin((*word_token)->origin, token->origin);
	(*word_token)->quoting_ctx = ft_strjoin((*word_token)->quoting_ctx,
			token->quoting_ctx);
	(*word_token)->expansion_ctx = ft_strjoin((*word_token)->expansion_ctx,
			token->expansion_ctx);
}

static void	_join_or_acc(t_token *token, t_list *new_tokens,
	t_token *word_token[static 1])
{
	if (!token->is_last_token)
		_join_word_token(word_token, token);
	else if (token->type == TOK_WORD || token->type == TOK_SQUOTE_STR
		|| token->type == TOK_DQUOTE_STR)
	{
		_join_word_token(word_token, token);
		lpush(new_tokens, as_token(*word_token));
		*word_token = NULL;
	}
	else
	{
		if (*word_token)
		{
			lpush(new_tokens, as_token(*word_token));
			*word_token = NULL;
		}
		lpush(new_tokens, as_token(token));
	}
}

void	join_tokens(t_list *tokens)
{
	t_list_node	*first;
	t_list		*new_tokens;
	t_token		*token;
	t_token		*word_token;

	new_tokens = lnew();
	first = tokens->first;
	if (!first)
		return ;
	token = first->as_token;
	word_token = NULL;
	if (token->is_last_token)
		lpush(new_tokens, as_token(token));
	else
		word_token = new_token(token->str, TOK_WORD, true);
	while (first->next != tokens->first)
	{
		first = first->next;
		_join_or_acc(first->as_token, new_tokens, &word_token);
	}
	tokens->first = new_tokens->first;
	tokens->last = new_tokens->last;
	tokens->len = new_tokens->len;
}
