/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:00 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 21:38:08 by tischmid         ###   ########.fr       */
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
