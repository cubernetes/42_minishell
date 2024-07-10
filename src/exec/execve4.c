/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:24:58 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 17:53:48 by tischmid         ###   ########.fr       */
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

void	msh_xtrace(char *const argv[])
{
	t_list	*args;
	char	*xtrace_fd_str;
	int		xtrace_fd;

	args = lnew();
	while (*argv)
	{
		if (force_ansi_c_quoting(*argv))
			lpush(args, as_str(quote_ansi_c(*argv, false)));
		else if (force_single_quoting(*argv))
			lpush(args, as_str(quote_single(*argv)));
		else
			lpush(args, as_str(*argv));
		++argv;
	}
	xtrace_fd_str = var_lookup("MINISHELL_XTRACEFD");
	if (xtrace_fd_str[0] == '\0')
		xtrace_fd_str = ft_itoa(MINISHELL_DEFAULT_XTRACEFD);
	xtrace_fd = ft_atoi(xtrace_fd_str);
	ft_dprintf(xtrace_fd, ft_strjoin(var_lookup("PS4"), ljoin(args, " ")));
	ft_dprintf(xtrace_fd, "\n");
}

char	**transform_for_declare(t_tree *simple_command)
{
	char	**argv;
	t_list	*d_argv;
	int		i;

	d_argv = lnew();
	lpush(d_argv, as_str("declare"));
	lpush(d_argv, as_str("--"));
	liter(simple_command->children);
	while (lnext(simple_command->children))
		if (simple_command->children->current->as_tree->type == TOKEN)
			lpush(d_argv,
				as_str(simple_command->children->current->as_tree->token->str));
	argv = ft_malloc(sizeof(*argv) * (d_argv->len + 1));
	i = 0;
	liter(d_argv);
	while (lnext(d_argv))
		argv[i++] = d_argv->current->as_str;
	argv[i] = NULL;
	// if (i > 0)
	// 	set_var("_", argv[i - 1], (t_flags){0});
	return (argv);
}

bool	is_valid_assignment_word(t_token *token, char *word)
{
	int		idx;
	char	quoting_ctx;
	char	escape_ctx;
	char	expansion_ctx;

	quoting_ctx = token->quoting_ctx[idx];
	expansion_ctx = token->expansion_ctx[idx];
	escape_ctx = token->escape_ctx[idx];
	idx = -1;
	while (word[++idx])
	{
		if (escape_ctx == '1' || quoting_ctx == '1' || expansion_ctx == '1')
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

bool	is_assignment_word(t_token *token)
{
	t_list	*split_token;
	char	*word;

	split_token = lsplit(token->str, "=");
	if (split_token->len <= 1)
		return (false);
	word = split_token->first->as_str;
	return (is_valid_assignment_word(token, word));
}

int	count_assignment_words(t_tree *simple_command)
{
	int		assignment_words;
	t_token	*token;

	assignment_words = 0;
	liter(simple_command->children);
	while (lnext(simple_command->children))
	{
		if (simple_command->children->current->as_tree->type == TOKEN)
		{
			token = simple_command->children->current->as_tree->token;
			if (is_assignment_word(token))
				assignment_words++;
			else
				return (-1);
		}
	}
	return (assignment_words);
}
