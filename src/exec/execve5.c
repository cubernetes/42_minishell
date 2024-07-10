/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 00:02:50 by paul              #+#    #+#             */
/*   Updated: 2024/07/09 00:03:36 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <stdio.h>
#include <readline/readline.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/* TODO: What to do in case of execve error? */
#define EXECVE_ERR 3
#define FORK_ERROR 4

bool	is_valid_assignment(char *word, t_token *token)
{
	int	idx;

	idx = -1;
	while (word[++idx])
	{
		if (token->escape_ctx[idx] == '1'
			|| token->quoting_ctx[idx] == '1'
			|| token->expansion_ctx[idx] == '1')
			return (false);
		if (ft_isalnum(word[idx]) || word[idx] == '_')
		{
			if (idx == 0 && ft_isdigit(word[idx]))
				return (false);
		}
		else
			return (false);
	}
	return (true);
}

bool	is_only_assigment_words(t_tree *simple_command)
{
	t_list	*split_token;
	t_token	*token;
	char	*word;
	int		assignment_words;

	assignment_words = 0;
	liter(simple_command->children);
	while (lnext(simple_command->children))
	{
		if (simple_command->children->current->as_tree->type == TOKEN)
		{
			token = simple_command->children->current->as_tree->token;
			split_token = lsplit(token->str, "=");
			if (split_token->len > 1)
			{
				word = split_token->first->as_str;
				if (!is_valid_assignment(word, token))
					return (false);
				++assignment_words;
			}
			else
				return (false);
		}
	}
	return ((bool)assignment_words);
}
