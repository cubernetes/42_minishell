/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:18:06 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 20:21:58 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_list	*handle_dollar(t_token **tok, t_token *token, int *start, int idx)
{
	t_list	*subwords;

	subwords = lnew();
	*tok = new_token(ft_strndup(token->str + *start, (size_t)(idx - *start)),
			TOK_WORD, true);
	(*tok)->escape_ctx = ft_strndup(token->escape_ctx + *start, (size_t)(idx
				- *start + 1));
	lpush(subwords, as_token(*tok));
	*start = idx;
	return (subwords);
}

t_list	*handle_special_param(t_special_param *args)
{
	t_list	*subwords;

	subwords = lnew();
	*args->tok = new_token(ft_strndup(args->token->str + *args->start,
				(size_t)(args->idx - *args->start + 1)), TOK_WORD, true);
	(*args->tok)->escape_ctx = ft_strndup(args->token->escape_ctx
			+ *args->start, (size_t)(args->idx - *args->start + 2));
	lpush(subwords, as_token(*args->tok));
	*args->start = args->idx + 1;
	*args->parse_param = 0;
	return (subwords);
}

t_list	*handle_end_of_param(t_special_param *args)
{
	t_list	*subwords;

	subwords = lnew();
	*args->tok = new_token(ft_strndup(args->token->str + *args->start,
				(size_t)(args->idx - *args->start)), TOK_WORD, true);
	(*args->tok)->escape_ctx = ft_strndup(args->token->escape_ctx
			+ *args->start, (size_t)(args->idx - *args->start + 1));
	lpush(subwords, as_token(*args->tok));
	*args->start = args->idx;
	*args->parse_param = 0;
	return (subwords);
}

t_list	*handle_param(t_special_param *args)
{
	t_list	*subwords;

	subwords = lnew();
	if (args->token->escape_ctx[args->idx] == '1')
		*args->parse_param = 0;
	if (*args->parse_param == 1 && (ft_isalpha(args->token->str[args->idx])
			|| args->token->str[args->idx] == '_'))
		++*args->parse_param;
	else if (*args->parse_param == 1 && ft_strchr("?$-0123456789#",
			args->token->str[args->idx]))
		lextend(subwords, handle_special_param(&(t_special_param){args->tok,
				args->token, args->start, args->idx, args->parse_param}));
	else if (*args->parse_param > 1 && (ft_isalpha(args->token->str[args->idx])
			|| ft_isdigit(args->token->str[args->idx])
			|| args->token->str[args->idx] == '_'))
		++*args->parse_param;
	else
		lextend(subwords, handle_end_of_param(&(t_special_param){args->tok,
				args->token, args->start, args->idx, args->parse_param}));
	return (subwords);
}

t_list	*split_into_subwords(t_token *token)
{
	t_list	*subwords;
	int		idx;
	int		start;
	int		parse_param;
	t_token	*tok;

	subwords = lnew();
	idx = -1;
	start = 0;
	parse_param = 0;
	while (token->str[++idx])
	{
		if (token->str[idx] == '$' && token->escape_ctx[idx] == '0'
			&& (lextend(subwords, handle_dollar(&tok, token, &start, idx)), 1))
			parse_param = 1;
		else if (parse_param > 0)
			lextend(subwords, handle_param(&(t_special_param){&tok, token,
					&start, idx, &parse_param}));
	}
	tok = new_token(ft_strndup(token->str + start, (size_t)(idx - start)),
			TOK_WORD, true);
	tok->escape_ctx = ft_strndup(token->escape_ctx + start, (size_t)(idx - start
				+ 1));
	lpush(subwords, as_token(tok));
	return (subwords);
}
