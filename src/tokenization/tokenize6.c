/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 00:31:27 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 18:09:03 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

bool	tokenize_variable_len_tokens(const char **line, t_list *tokens)
{
	bool	pushed;

	pushed = false;
	pushed |= tokenize_single_quoted_string(line, tokens);
	pushed |= tokenize_double_quoted_string(line, tokens);
	pushed |= tokenize_word(line, tokens);
	if (!pushed)
		return (minishell_error(EXIT_FAILURE, false, false,
				"could not tokenize `%s'",
				get_token_str(tokens)), false);
	return (true);
}

/* TODO: set is_last_token member for each token */
t_list	*tokenize(const char *line)
{
	t_list	*tokens;

	tokens = lnew();
	skip_space_tab(&line);
	while (true)
	{
		if (!tokenize_fixed_len_tokens(&line, tokens))
			if (!tokenize_variable_len_tokens(&line, tokens))
				return (NULL);
		if (tokens->last->as_token->type == TOK_EOL)
			break ;
		skip_space_tab(&line);
	}
	return (tokens);
}
