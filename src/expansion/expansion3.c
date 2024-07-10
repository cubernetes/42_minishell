/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:18:31 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 17:39:49 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/* start inclusive, end exclusive */
t_token	*new_word_token(t_token *token, int start, int end)
{
	t_token	*new_tok;

	new_tok = new_token(ft_strndup(token->str + start,
				(size_t)(end - start)), TOK_WORD, true);
	new_tok->quoting_ctx = ft_strndup(token->quoting_ctx + start,
			(size_t)(end - start));
	new_tok->expansion_ctx = ft_strndup(token->expansion_ctx + start,
			(size_t)(end - start));
	new_tok->escape_ctx = ft_strndup(token->escape_ctx + start,
			(size_t)(end - start) + 1);
	return (new_tok);
}

t_list	*handle_ifs_whitespace(bool *push, t_token *token,
	int *start, int idx)
{
	t_list	*words;

	words = lnew();
	if (*push == true)
	{
		lpush(words,
			as_token(new_word_token(token, *start, idx)));
		*push = false;
	}
	return (words);
}

t_list	*handle_ifs_non_whitespace(bool *push, t_token *token,
	int *start, int idx)
{
	t_list	*words;
	char	*ifs;

	ifs = get_ifs();
	words = lnew();
	if (*push == true)
	{
		lpush(words,
			as_token(new_word_token(token, *start, idx)));
		*start = idx;
		*push = false;
	}
	else
	{
		if (*start == -1 || (ft_strchr(ifs, token->str[*start])
				&& !ft_strchr(IFS_WHITESPACE,
					token->str[*start])))
			lpush(words,
				as_token(new_token("", TOK_WORD, true)));
		else
			*start = idx;
	}
	return (words);
}

t_list	*handle_ifs(bool *push, t_token *token, int *start, int idx)
{
	t_list	*words;

	words = lnew();
	if (ft_strchr(IFS_WHITESPACE, token->str[idx]))
		lextend(words, handle_ifs_whitespace(push, token, start, idx));
	else
		lextend(words, handle_ifs_non_whitespace(push, token, start, idx));
	return (words);
}

t_list	*handle_expansion_ctx(bool *push, t_token *token,
	int *start, int idx)
{
	char	*ifs;
	t_list	*words;

	ifs = get_ifs();
	words = lnew();
	if (ft_strchr(ifs, token->str[idx]))
		lextend(words, handle_ifs(push, token, start, idx));
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
