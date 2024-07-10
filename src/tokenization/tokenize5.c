/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 00:23:53 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 18:10:20 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void	tokenize_double_quoted_string_helper(size_t len, const char *tmp,
const char **line, t_list *tokens)
{
	if (*tmp == '"')
	{
		++(*line);
		push_token(line, tokens, len, TOK_DQUOTE_STR);
		++(*line);
	}
	else
		push_token(line, tokens, len + 1, TOK_ERROR);
}

bool	tokenize_double_quoted_string(const char **line, t_list *tokens)
{
	size_t		len;
	const char	*tmp;
	bool		escaped;

	if (**line == '"')
	{
		len = 0;
		tmp = *line + 1;
		escaped = false;
		while ((escaped || *tmp != '"') && *tmp != '\0')
		{
			if (escaped == true)
				escaped = false;
			else if (*tmp == '\\')
				escaped = true;
			++tmp;
			++len;
		}
		tokenize_double_quoted_string_helper(len, tmp, line, tokens);
		return (true);
	}
	return (false);
}

/* Not including `&' since we do not have job control */
bool	is_word_char(char c)
{
	return ((bool)(
		c
		&& c != ' ' && c != '\t'
		&& !ft_strchr(";><()'\"|", c)
	));
}

bool	is_not_and_and(const char *line)
{
	if (line[0] == '&' && line[1] == '&')
		return (false);
	return (true);
}

bool	tokenize_word(const char **line, t_list *tokens)
{
	size_t		len;
	const char	*tmp;
	bool		escaped;

	if (**line == '#')
		return (push_token(line, tokens, 0, TOK_EOL));
	if (is_word_char(**line) && is_not_and_and(*line))
	{
		len = 0;
		tmp = *line;
		escaped = false;
		while ((escaped || (is_word_char(*tmp) && is_not_and_and(tmp)))
			&& *tmp != '\0')
		{
			if (escaped == true)
				escaped = false;
			else if (*tmp == '\\')
				escaped = true;
			++tmp;
			++len;
		}
		push_token(line, tokens, len, TOK_WORD);
		return (true);
	}
	return (false);
}
