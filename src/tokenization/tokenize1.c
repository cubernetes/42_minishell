/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:36:14 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 18:08:05 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

const char	*token_type_to_string_1(t_token_type type)
{
	if (type == TOK_OVERRIDE)
		return (STR_TOK_OVERRIDE);
	else if (type == TOK_OVERRIDE_ERR)
		return (STR_TOK_OVERRIDE_ERR);
	else if (type == TOK_INPUT)
		return (STR_TOK_INPUT);
	else if (type == TOK_APPEND)
		return (STR_TOK_APPEND);
	else if (type == TOK_APPEND_ERR)
		return (STR_TOK_APPEND_ERR);
	else if (type == TOK_HEREDOC)
		return (STR_TOK_HEREDOC);
	else if (type == TOK_PIPE)
		return (STR_TOK_PIPE);
	else if (type == TOK_AND)
		return (STR_TOK_AND);
	else if (type == TOK_OR)
		return (STR_TOK_OR);
	else if (type == TOK_SEMI)
		return (STR_TOK_SEMI);
	return (NULL);
}

const char	*token_type_to_string_2(t_token_type type)
{
	if (type == TOK_L_PAREN)
		return (STR_TOK_L_PAREN);
	else if (type == TOK_R_PAREN)
		return (STR_TOK_R_PAREN);
	else if (type == TOK_SQUOTE_STR)
		return (STR_TOK_SQUOTE_STR);
	else if (type == TOK_DQUOTE_STR)
		return (STR_TOK_DQUOTE_STR);
	else if (type == TOK_WORD)
		return (STR_TOK_WORD);
	else if (type == TOK_EOL)
		return (STR_TOK_EOL);
	else if (type == TOK_EPSILON)
		return (STR_TOK_EPSILON);
	else if (type == TOK_ERROR)
		return (STR_TOK_ERROR);
	return (STR_TOK_UNKNOWN);
}

/* TODO: Not required: hashtable */
const char	*token_type_to_string(t_token_type type)
{
	const char	*str_type;

	str_type = token_type_to_string_1(type);
	if (str_type)
		return (str_type);
	str_type = token_type_to_string_2(type);
	return (str_type);
}

void	skip_space_tab(const char **line)
{
	while (**line == ' ' || **line == '\t')
		++(*line);
}
