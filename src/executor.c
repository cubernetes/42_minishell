/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:30:54 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:30:55 by paul             ###   ########.fr       */
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

void	close_fd(int fd1, int fd2, char *file_path, t_tree *simple_command)
{
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
	simple_command->error = minishell_error(1, false,
			false, "%s: %s", file_path, strerror(errno));
	unlink(file_path);
}

void	handle_successful_read(t_successful_read_params *params)
{
	t_list	*tokens;
	int		sc_fd_in;

	tokens = NULL;
	ft_dprintf(params->new_hd_fd, "%s", tokens->first->as_token->str);
	close(params->new_hd_fd);
	close(params->fd);
	params->new_hd_fd = open(params->new_hd, O_RDONLY);
	unlink(params->new_hd);
	unlink(params->file_path);
	if (params->new_hd_fd == -1)
	{
		params->simple_command->error = minishell_error(1, false,
				false, "%s: %s", params->file_path, strerror(errno));
		return ;
	}
	sc_fd_in = params->simple_command->fd_in;
	if (sc_fd_in != -2)
		close(sc_fd_in);
	params->simple_command->fd_in = params->new_hd_fd;
	params->simple_command->error = 0;
}

static void	init_read_params(t_successful_read_params *params, int new_hd_fd,
int fd, char *new_hd)
{
	params->new_hd_fd = new_hd_fd;
	params->fd = fd;
	params->new_hd = new_hd;
}

void	redirect_heredoc(char *file_path, t_tree *simple_command)
{
	int							fd;
	char						*new_hd;
	int							new_hd_fd;
	t_successful_read_params	read_params;
	t_fatptr					line;

	fd = open(file_path, O_RDONLY);
	new_hd = ft_mktemp("minishell.");
	new_hd_fd = open(new_hd, O_CREAT | O_TRUNC | O_RDWR, 0600);
	if (fd == -1 || new_hd_fd == -1)
		close_fd(fd, new_hd_fd, file_path, simple_command);
	else
	{
		init_read_params(&read_params, new_hd_fd, fd, new_hd);
		read_params.simple_command = simple_command;
		read_params.file_path = file_path;
		while (1)
		{
			line = get_next_fat_line(fd);
			if (!line.data)
				break ;
			handle_successful_read(&read_params);
		}
	}
}

void	handle_redirect_input(char *file_path, t_tree *simple_command)
{
	int	fd;
	int	sc_fd_in;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		simple_command->error = minishell_error(1, false, false,
				"%s: %s", file_path, strerror(errno));
	else
	{
		sc_fd_in = simple_command->fd_in;
		if (sc_fd_in != -2)
			close(sc_fd_in);
		simple_command->fd_in = fd;
		simple_command->error = 0;
	}
}
