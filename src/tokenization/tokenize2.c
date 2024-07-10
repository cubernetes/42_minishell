/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:37:46 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 19:30:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

	/* sep = "\t|\t"; */
void	print_token_debug(t_data data, int n)
{
	t_token	*token;
	char	*clr;
	char	*sep;

	sep = "|";
	token = data.as_token;
	if (token->is_last_token)
		clr = "";
	else
		clr = "\033[33m";
	if (n == 0)
		ft_printf("<\033[31m%s\033[m>%s%s%s\033[m%s%d%s<%s>%s<%s>%s<%s>%s<%s>",
			token->str, sep, clr,
			token_type_to_string(token->type), sep,
			token->num_tokens_after_split, sep, token->expansion_ctx, sep,
			token->quoting_ctx, sep, token->escape_ctx, sep, token->origin);
	else
		ft_printf("\n<\033[31m%s\033[m>%s%s%s\033[m%s%d%s<%s>%s"
			"<%s>%s<%s>%s<%s>",
			token->str, sep, clr,
			token_type_to_string(token->type), sep,
			token->num_tokens_after_split, sep, token->expansion_ctx, sep,
			token->quoting_ctx, sep, token->escape_ctx, sep, token->origin);
}

void	print_token(t_data data, int n)
{
	t_token	*token;
	char	*clr;

	token = data.as_token;
	if (token->is_last_token)
		clr = "";
	else
		clr = "\033[33m";
	if (n == 0)
		ft_printf("<\033[31m%s\033[m> (%s%s\033[m)", token->str, clr,
			token_type_to_string(token->type));
	else
		ft_printf(" -> <\033[31m%s\033[m> (%s%s\033[m)", token->str, clr,
			token_type_to_string(token->type));
}

char	*was_quoted(t_token_type type)
{
	if (type == TOK_SQUOTE_STR || type == TOK_DQUOTE_STR)
		return ("1");
	return ("0");
}

char	*repeat_string(char *str, size_t n)
{
	char	*res;

	res = "";
	while (n--)
		res = ft_strjoin(res, str);
	return (res);
}

/* TODO: change return type to t_token later */
void	*new_token(char *str, t_token_type type, bool is_last_token)
{
	int	s;

	s = 1;
	if (type == TOK_EOL)
		s = 0;
	return (ft_memdup(\
		&(t_token){
			.str = str,
			.type = type,
			.is_last_token = is_last_token,
			.quoting_ctx = repeat_string(was_quoted(type), ft_strlen(str)),
			.expansion_ctx = repeat_string("0", ft_strlen(str)),
			.num_tokens_after_split = s,
			.escape_ctx = repeat_string("0", ft_strlen(str) + 1),
			.origin = ""
		},
		sizeof(t_token)
	));
}
