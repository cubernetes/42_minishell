/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:00 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 20:09:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/* TODO: Test for unused headers */
#include <errno.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> /* dup() */
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

void	handle_redirect_append(char *file_path,
t_tree *simple_command, bool red_err)
{
	int		fd;
	int		sc_fd;

	fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0666 & ~ft_getumask());
	if (fd == -1)
		simple_command->error = minishell_error(1, 0,
				"%s: %s", file_path, strerror(errno));
	else
	{
		if (red_err)
			sc_fd = simple_command->fd_err;
		else
			sc_fd = simple_command->fd_out;
		if (sc_fd != -2)
			close(sc_fd);
		if (red_err)
			simple_command->fd_err = fd;
		else
			simple_command->fd_out = fd;
		simple_command->error = 0;
	}
}

void	assign_fd_and_clear_error(int fd, t_tree *simple_command, bool red_err)
{
	int	sc_fd;

	if (red_err)
		sc_fd = simple_command->fd_err;
	else
		sc_fd = simple_command->fd_out;
	if (sc_fd != -2)
		close(sc_fd);
	if (red_err)
		simple_command->fd_err = fd;
	else
		simple_command->fd_out = fd;
	simple_command->error = 0;
}

void	handle_redirect_override(char *file_path, t_tree *simple_command,
bool red_err)
{
	int		fd;
	int		oflag;
	char	*msg;

	oflag = O_WRONLY | O_TRUNC | O_CREAT;
	if (option_enabled('C'))
		oflag |= O_EXCL;
	fd = open(file_path, oflag, 0666 & ~ft_getumask());
	if (fd == -1)
	{
		if (option_enabled('C'))
			msg = "cannot overwrite existing file";
		else
			msg = strerror(errno);
		simple_command->error = minishell_error(1, 0,
				"%s: %s", file_path, msg);
	}
	else
		assign_fd_and_clear_error(fd, simple_command, red_err);
}

int	handle_io_redirect(t_tree *io_redirect, t_tree *simple_command)
{
	char			*file_path;
	t_token_type	type;

	type = io_redirect->children->first->as_tree->token->type;
	file_path = io_redirect->children->first->next->as_tree->token->str;
	if (type == TOK_OVERRIDE)
		handle_redirect_override(file_path, simple_command, false);
	else if (type == TOK_OVERRIDE_ERR)
		handle_redirect_override(file_path, simple_command, true);
	else if (type == TOK_INPUT)
		handle_redirect_input(file_path, simple_command);
	else if (type == TOK_APPEND)
		handle_redirect_append(file_path, simple_command, false);
	else if (type == TOK_APPEND_ERR)
		handle_redirect_append(file_path, simple_command, true);
	else if (type == TOK_HEREDOC)
		redirect_heredoc(file_path, simple_command);
	return (simple_command->error);
}

/* TODO: get rid of this wrapper mess */
/* TOOD: abstract away into new function */
pid_t	execute_simple_command_wrapper(t_tree *simple_command,
	t_list *commands)
{
	t_list	*new_chldn;
	t_list	*cleaned_tokens;

	new_chldn = lnew();
	liter(simple_command->children);
	while (lnext(simple_command->children))
	{
		if (simple_command->children->current->as_tree->type == IO_REDIRECT
			&& liter(simple_command->children->current->as_tree->children))
			while (lnext(simple_command->children->current->as_tree->children))
				lpush(new_chldn, as_token(simple_command->children->current
						->as_tree->children->current->as_tree->token));
		else
			lpush(new_chldn, as_token(simple_command->children->current
					->as_tree->token));
	}
	lpush(new_chldn, as_token(new_token("", TOK_EOL, true)));
	new_chldn = expand_tokens(new_chldn);
	if (new_chldn == NULL)
		return (-256);
	join_tokens(new_chldn);
	if (!option_enabled('f'))
		glob_tokens(new_chldn);
	cleaned_tokens = lnew();
	liter(new_chldn);
	while (lnext(new_chldn))
	{
		if ((new_chldn->current->as_token->type == TOK_INPUT
				|| new_chldn->current->as_token->type == TOK_OVERRIDE
				|| new_chldn->current->as_token->type == TOK_APPEND
				|| new_chldn->current->as_token->type == TOK_HEREDOC)
			&& new_chldn->current->next->as_token->type == TOK_WORD
			&& new_chldn->current->next->as_token->num_tokens_after_split == 0)
			return (close_fds(simple_command), minishell_error(1, 0,
					"%s: ambiguous redirect",
					new_chldn->current->next->as_token->origin) - 257);
		if (new_chldn->current->as_token->num_tokens_after_split != 0
			|| new_chldn->current->as_token->type == TOK_EOL)
			lpush(cleaned_tokens, as_data(new_chldn->current));
	}
	new_chldn->first = cleaned_tokens->first;
	new_chldn->last = cleaned_tokens->last;
	new_chldn->len = cleaned_tokens->len;
	if (new_chldn->len <= 1)
		return (close_fds(simple_command), -258);
	new_chldn = build_ast(new_chldn, false)->children->first->as_tree
		->children->first->as_tree->children;
	simple_command->children = new_chldn;
	liter(simple_command->children);
	while (lnext(simple_command->children))
		if (simple_command->children->current->as_tree->type == IO_REDIRECT)
			if (handle_io_redirect(simple_command->children->current->as_tree,
					simple_command))
				return (close_fds(simple_command), simple_command->error - 257);
	return (execute_simple_command(simple_command, commands));
}
