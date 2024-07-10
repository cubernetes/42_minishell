/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:43:20 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 18:10:01 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void	process_backslash(t_list *result_chars, t_list *escape_ctx_chars,
	const char **str, char terminator)
{
	if ((*str)[1])
	{
		if (terminator == '\"' && !ft_strchr("\"$\\", (*str)[1]))
		{
			lpush(result_chars, as_str("\\"));
			lpush(escape_ctx_chars, as_str("0"));
		}
		lpush(escape_ctx_chars, as_str("1"));
		lpush(result_chars, as_str(ft_strndup(*str + 1, 1)));
		++(*str);
	}
	else
		lpush(escape_ctx_chars, as_str("1"));
}
// TODO: Handle line continuations

void	process_char(t_list *result_chars, t_list *escape_ctx_chars,
	const char **str)
{
	lpush(result_chars, as_str(ft_strndup(*str, 1)));
	lpush(escape_ctx_chars, as_str("0"));
}

char	*process_backslashes_helper(char *str, char terminator,
	char **escape_ctx)
{
	t_list	*result_chars;
	t_list	*escape_ctx_chars;

	escape_ctx_chars = lnew();
	result_chars = lnew();
	while (*str)
	{
		if (*str == '\\')
			process_backslash(result_chars, escape_ctx_chars,
				(const char **)&str, terminator);
		else
			process_char(result_chars, escape_ctx_chars, (const char **)&str);
		str++;
	}
	if (escape_ctx_chars->len == result_chars->len)
		lpush(escape_ctx_chars, as_str("0"));
	*escape_ctx = ljoin(escape_ctx_chars, "");
	return (ljoin(result_chars, ""));
}

char	*process_backslashes(char *str, t_token_type type,
	char **escape_ctx)
{
	if (type == TOK_DQUOTE_STR)
		return (process_backslashes_helper(str, '\"', escape_ctx));
	else if (type == TOK_WORD)
		return (process_backslashes_helper(str, '\0', escape_ctx));
	else
		return (str);
}

void	init_token_attrs(t_token_attrs *attrs, const char *line,
	size_t token_len, t_token_type type)
{
	attrs->str = process_backslashes(ft_strndup(line, token_len), type,
			&attrs->escape_ctx);
	attrs->is_str_tok = type == TOK_SQUOTE_STR || type == TOK_DQUOTE_STR
		|| type == TOK_WORD;
	attrs->is_last = ft_isspace(line[token_len]) || type == TOK_EOL
		|| line[token_len] == '\0';
	if (type == TOK_SQUOTE_STR || type == TOK_DQUOTE_STR)
		attrs->is_last_quote = (line[token_len + 1] == '\0'
				|| ft_isspace(line[token_len + 1]));
	else
		attrs->is_last_quote = false;
	attrs->is_last_token = attrs->is_last
		|| attrs->is_last_quote || !attrs->is_str_tok;
}
