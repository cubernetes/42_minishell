/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:08:35 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 17:41:15 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	handle_word_token(t_token *joined_token, t_list *new_tokens,
	t_list *tokens)
{
	t_list	*split_tokens;

	if (joined_token->str[0] != '\0' && ft_strchr(get_ifs(),
			joined_token->str[0]))
		if (new_tokens->last != NULL
			&& new_tokens->last->as_token->is_last_token == false)
			new_tokens->last->as_token->is_last_token = true;
	split_tokens = liter(split_into_words(joined_token));
	while (lnext(split_tokens))
		split_tokens->current->as_token->origin
			= tokens->current->as_token->origin;
	lextend(new_tokens, lcopy(split_tokens));
	if (split_tokens->last->as_token->num_tokens_after_split == 0
		|| !ft_strchr(get_ifs(),
			joined_token->str[ft_strlen(joined_token->str) - 1])
		|| joined_token->escape_ctx[ft_strlen(joined_token->str) - 1]
		== '1')
		if (tokens->current->as_token->is_last_token == false)
			new_tokens->last->as_token->is_last_token = false;
}

void	handle_dquote_str_token(t_token *joined_token, t_list *new_tokens,
	t_list *tokens)
{
	joined_token->type = TOK_DQUOTE_STR;
	joined_token->quoting_ctx = repeat_string("1",
			ft_strlen(joined_token->str));
	joined_token->origin = tokens->current->as_token->origin;
	lpush(new_tokens, as_token(joined_token));
	if (tokens->current->as_token->is_last_token == false)
		new_tokens->last->as_token->is_last_token = false;
}

t_list	*expand_tokens(t_list *tokens)
{
	t_token	*joined_token;
	t_list	*subwords;
	t_list	*new_tokens;

	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
	{
		if (tokens->current->as_token->type != TOK_WORD
			&& tokens->current->as_token->type != TOK_DQUOTE_STR
			&& (lpush(new_tokens, as_data(tokens->current)), 1))
			continue ;
		subwords = split_into_subwords(ft_tilde_expand(
					tokens->current->as_token));
		subwords = expand_subwords(subwords);
		if (subwords == NULL)
			return (NULL);
		subwords = liter(remove_empty_words(subwords));
		joined_token = gen_joined_token(subwords);
		if (tokens->current->as_token->type == TOK_WORD)
			handle_word_token(joined_token, new_tokens, tokens);
		else
			handle_dquote_str_token(joined_token, new_tokens, tokens);
	}
	return (new_tokens);
}
