/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:17:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 17:38:14 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*var_lookup(char *key)
{
	t_var	*var;

	var = get_var(key);
	if (var && var->value)
		return (var->value);
	return ("");
}

char	*get_ifs(void)
{
	t_var	*ifs_var;

	ifs_var = get_var("IFS");
	if (ifs_var == NULL || ifs_var->value == NULL)
		return (DEFAULT_IFS);
	return (ifs_var->value);
}

t_list	*remove_empty_words(t_list	*words)
{
	t_list	*new_words;

	new_words = lnew();
	liter(words);
	while (lnext(words))
		if (words->current->as_token->str[0] != '\0')
			lpush(new_words, as_data(words->current));
	if (words->first != NULL && new_words->len == 0)
		lpush(new_words, as_data(words->first));
	return (new_words);
}

bool	expand_parameter(t_list *words, t_list *new_words)
{
	t_var	*var;
	char	*var_str;
	t_token	*expanded_token;

	if (words->current->as_token->str[1] == '\0')
		lpush(new_words, as_data(words->current));
	else
	{
		var = get_var(words->current->as_token->str + 1);
		if (var && var->value)
			var_str = var->value;
		else if (option_enabled('u'))
			return (minishell_error(1, false, false,
					"%s: unbound variable",
					words->current->as_token->str + 1), false);
		else
			var_str = "";
		expanded_token = new_token(var_str, TOK_WORD, true);
		expanded_token->expansion_ctx = repeat_string("1",
				ft_strlen(expanded_token->str));
		expanded_token->escape_ctx = repeat_string("0",
				ft_strlen(expanded_token->str) + 1);
		lpush(new_words, as_token(expanded_token));
	}
	return (true);
}

t_list	*expand_subwords(t_list	*words)
{
	t_list	*new_words;

	new_words = lnew();
	liter(words);
	while (lnext(words))
	{
		if (words->current->as_token->str[0] == '$'
			&& words->current->as_token->escape_ctx[0] == '0')
		{
			if (expand_parameter(words, new_words) == false)
				return (NULL);
		}
		else
		{
			words->current->as_token->expansion_ctx = repeat_string("0",
					ft_strlen(words->current->as_token->str));
			lpush(new_words, as_data(words->current));
		}
	}
	return (new_words);
}
