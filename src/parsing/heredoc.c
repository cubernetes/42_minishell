/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:34:17 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 18:30:19 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft.h"

#include <stdio.h>
#include <readline/readline.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

bool	heredoc_aborted(int op)
{
	static bool	aborted = false;

	if (op == 1)
		aborted = true;
	else if (op == 0)
		aborted = false;
	return (aborted);
}

static bool	_redirect_err(int *old, char *ps2[static 1])
{
	int			tty;

	tty = open("/dev/tty", O_WRONLY);
	if (tty == -1)
		(void)minishell_error(1, false, false, "/dev/tty: ",
			strerror(errno));
	else
	{
		*old = dup(STDERR_FILENO);
		dup2(tty, STDERR_FILENO);
		close(tty);
	}
	if (!option_enabled('i'))
		*ps2 = "";
	return (true);
}

static bool	_handle_interactive_heredoc(int fd, char *ps2, char *delimiter,
	char *input[static 1])
{
	bool		restore;
	int			old;

	restore = false;
	if (!isatty(STDERR_FILENO))
		restore = _redirect_err(&old, &ps2);
	interactive_signals();
	*input = readline(ps2);
	noninteractive_signals();
	if (restore)
	{
		dup2(old, STDERR_FILENO);
		close(old);
	}
	gc_add(*input);
	if (*input == NULL || !ft_strcmp(*input, delimiter) || heredoc_aborted(-1))
		return (true);
	write(fd, *input, ft_strlen(*input));
	write(fd, "\n", 1);
	return (false);
}

static bool	_handle_noninteractive_heredoc(int fd, char *ps2, char *delimiter,
	char *input[static 1])
{
	t_fatptr	input_gnl;

	noninteractive_signals();
	input_gnl = get_next_fat_line(STDIN_FILENO);
	if (option_enabled('i') && input_gnl.data != NULL)
	{
		ft_dprintf(STDERR_FILENO, "%s", ft_strjoin(ps2, input_gnl.data));
		if (((char *)input_gnl.data)[input_gnl.size - 2] != '\n')
			ft_dprintf(STDERR_FILENO, "%s", "\n");
	}
	else if (option_enabled('i'))
		ft_dprintf(STDERR_FILENO, "%s", ps2);
	if (input_gnl.data == NULL)
		*input = NULL;
	if (*input != NULL
		&& ((char *)input_gnl.data)[ft_strlen(input_gnl.data) - 1] == '\n')
		((char *)input_gnl.data)[ft_strlen(input_gnl.data) - 1] = '\0';
	if (input_gnl.data == NULL || !ft_strcmp(input_gnl.data, delimiter)
		|| heredoc_aborted(-1))
		return (true);
	write(fd, input_gnl.data, input_gnl.size - 1);
	if (((char *)input_gnl.data)[input_gnl.size - 2] != '\n')
		write(fd, "\n", 1);
	return (false);
}

static void	_handle_error(char *delimiter)
{
	minishell_error(0, false, false,
		"warning: here-document at line %d delimited by "
		"end-of-file (wanted `%s')",
		ft_atoi(var_lookup("LINENO")),
		delimiter);
}

/* TODO: Check for error on write, open */
/* TOOD: heredoc in noninteractive mode */
char	*create_heredoc(char *delimiter)
{
	char		*heredoc_file;
	int			fd;
	char		*ps2;
	char		*input;
	bool		brk;

	if (heredoc_aborted(-1))
		return ("");
	heredoc_file = ft_mktemp("minishell.");
	fd = open(heredoc_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	while (1)
	{
		ps2 = expand_prompt(var_lookup("PS2"));
		if (isatty(STDIN_FILENO))
			brk = _handle_interactive_heredoc(fd, ps2, delimiter, &input);
		else
			brk = _handle_noninteractive_heredoc(fd, ps2, delimiter, &input);
		if (brk)
			break ;
	}
	close(fd);
	if (input == NULL)
		_handle_error(delimiter);
	return (heredoc_file);
}
