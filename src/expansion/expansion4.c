/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:18:59 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 17:39:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_list	*push_words(bool *push, t_token *token, int *start, int idx)
{
	t_list	*words;

	words = lnew();
	if (token->expansion_ctx[idx] == '1')
		lextend(words, handle_expansion_ctx(push, token, start, idx));
	else
	{
		if (*push == false)
		{
			*start = idx;
			*push = true;
		}
	}
	return (words);
}

t_list	*handle_post_splitting(bool push, t_token *token, int start,
	int idx)
{
	t_list	*words;
	char	*ifs;

	ifs = get_ifs();
	words = lnew();
	if (start != -1 && push == true && (token->expansion_ctx[start] == '0'
			|| !ft_strchr(ifs, token->str[start])))
		lpush(words, as_token(new_word_token(token, start, idx)));
	if (words->len == 0)
	{
		token = new_token("", TOK_WORD, true);
		token->num_tokens_after_split = 0;
		lpush(words, as_token(token));
	}
	else
	{
		liter(words);
		while (lnext(words))
			words->current->as_token->num_tokens_after_split = (int)words->len;
	}
	return (words);
}

/* shit... */
/* probably the most complex function in the entire codebase */
t_list	*split_into_words(t_token *token)
{
	t_list	*words;
	int		idx;
	int		start;
	bool	push;

	words = lnew();
	idx = -1;
	start = -1;
	push = false;
	while (token->str[++idx])
		lextend(words, push_words(&push, token, &start, idx));
	lextend(words, handle_post_splitting(push, token, start, idx));
	return (words);
}

t_token	*ft_tilde_expand(t_token *token)
{
	t_var	*home_var;

	if (token->str[0] != '~')
		return (token);
	if (token->escape_ctx[0] == '1')
		return (token);
	else if (token->type != TOK_WORD)
		return (token);
	else if (token->str[1] != '/' && token->str[1] != '\0')
		return (token);
	else if (token->escape_ctx[1] == '1' && token->str[1] != '\0')
		return (token);
	home_var = get_var("HOME");
	if (home_var == NULL)
		return (token);
	token->str = ft_strjoin(home_var->value, token->str + 1);
	return (token);
}

t_token	*gen_joined_token(t_list *subwords)
{
	t_token	*joined_token;

	joined_token = new_token("", TOK_WORD, true);
	joined_token->expansion_ctx = "";
	joined_token->escape_ctx = "";
	while (lnext(subwords))
	{
		joined_token->str = ft_strjoin(joined_token->str,
				subwords->current->as_token->str);
		joined_token->expansion_ctx
			= ft_strjoin(joined_token->expansion_ctx,
				subwords->current->as_token->expansion_ctx);
		joined_token->quoting_ctx = ft_strjoin(joined_token->quoting_ctx,
				subwords->current->as_token->quoting_ctx);
		joined_token->escape_ctx
			= ft_strjoin(ft_strndup(joined_token->escape_ctx,
					ft_strlen(joined_token->escape_ctx) - 1),
				subwords->current->as_token->escape_ctx);
	}
	return (joined_token);
}
