/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 22:19:10 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 00:19:10 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

bool	push_token(const char **line, t_list *tokens, size_t token_len,
t_token_type type)
{
	t_token_attrs	attrs;
	t_token			*token;

	init_token_attrs(&attrs, *line, token_len, type);
	token = new_token(attrs.str, type, attrs.is_last_token);
	token->escape_ctx = attrs.escape_ctx;
	token->origin = ft_strndup(*line, token_len);
	lpush(tokens, as_token(token));
	*line += token_len;
	return (true);
}

/* TODO: Not required: hashtable */
static bool	tokenize_2_or_3_char_tokens(const char **line, t_list *tokens)
{
	if (**line == '2' && *(*line + 1) == '>' && *(*line + 2) == '>')
		return (push_token(line, tokens, 3, TOK_APPEND_ERR));
	else if (**line == '2' && *(*line + 1) == '>')
		return (push_token(line, tokens, 2, TOK_OVERRIDE_ERR));
	else if (**line == '>' && *(*line + 1) == '>')
		return (push_token(line, tokens, 2, TOK_APPEND));
	else if (**line == '<' && *(*line + 1) == '<')
		return (push_token(line, tokens, 2, TOK_HEREDOC));
	else if (**line == '&' && *(*line + 1) == '&')
		return (push_token(line, tokens, 2, TOK_AND));
	else if (**line == '|' && *(*line + 1) == '|')
		return (push_token(line, tokens, 2, TOK_OR));
	return (false);
}

static bool	tokenize_1_char_tokens(const char **line, t_list *tokens)
{
	if (**line == ';')
		return (push_token(line, tokens, 1, TOK_SEMI));
	else if (**line == '>')
		return (push_token(line, tokens, 1, TOK_OVERRIDE));
	else if (**line == '<')
		return (push_token(line, tokens, 1, TOK_INPUT));
	else if (**line == '|')
		return (push_token(line, tokens, 1, TOK_PIPE));
	else if (**line == '(')
		return (push_token(line, tokens, 1, TOK_L_PAREN));
	else if (**line == ')')
		return (push_token(line, tokens, 1, TOK_R_PAREN));
	return (false);
}

bool	tokenize_fixed_len_tokens(const char **line, t_list *tokens)
{
	bool	pushed;

	if (**line == '\0')
		return (push_token(line, tokens, 0, TOK_EOL));
	pushed = tokenize_2_or_3_char_tokens(line, tokens);
	if (!pushed)
		pushed = tokenize_1_char_tokens(line, tokens);
	return (pushed);
}

bool	tokenize_single_quoted_string(const char **line, t_list *tokens)
{
	size_t		len;
	const char	*tmp;

	if (**line == '\'')
	{
		len = 0;
		tmp = *line + 1;
		while (*tmp != '\'' && *tmp != '\0')
		{
			++tmp;
			++len;
		}
		if (*tmp == '\'')
		{
			++(*line);
			push_token(line, tokens, len, TOK_SQUOTE_STR);
			++(*line);
		}
		else
			push_token(line, tokens, len + 1, TOK_ERROR);
		return (true);
	}
	return (false);
}
