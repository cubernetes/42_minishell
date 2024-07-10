/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:08:12 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 18:08:19 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*get_token_str(t_list *tokens)
{
	return (tokens->first->as_token->str);
}

char	*get_token_str_nl(t_list *tokens)
{
	char	*str;

	str = get_token_str(tokens);
	if (!*str)
		str = "newline";
	return (str);
}

t_token_type	get_token_type(t_list *tokens)
{
	if (tokens->first->as_token->type == TOK_SQUOTE_STR
		|| tokens->first->as_token->type == TOK_DQUOTE_STR)
		return (TOK_WORD);
	else if (tokens->first->as_token->type == TOK_SEMI)
		return (TOK_AND);
	else if (tokens->first->as_token->type == TOK_OVERRIDE_ERR)
		return (TOK_OVERRIDE);
	else if (tokens->first->as_token->type == TOK_APPEND_ERR)
		return (TOK_APPEND);
	return (tokens->first->as_token->type);
}
