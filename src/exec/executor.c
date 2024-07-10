/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:30:54 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 19:19:39 by tischmid         ###   ########.fr       */
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

void	redirect_heredoc(char *file_path, t_tree *simple_command)
{
	int			fd;
	int			new_hd_fd;
	int			sc_fd_in;
	t_fatptr	line;
	char		*new_hd;
	t_list		*tokens;

	fd = open(file_path, O_RDONLY);
	new_hd = ft_mktemp("minishell.");
	new_hd_fd = open(new_hd, O_CREAT | O_TRUNC | O_RDWR, 0600);
	if (fd == -1 || new_hd_fd == -1)
	{
		if (fd != -1)
			close(fd);
		if (new_hd_fd != -1)
			close(new_hd_fd);
		simple_command->error = minishell_error(1, false, false,
				"%s: %s", file_path, strerror(errno));
		(void)unlink(new_hd);
		(void)unlink(file_path);
	}
	else
	{
		while (1)
		{
			line = get_next_fat_line(fd);
			if (!line.data)
				break ;
			tokens = lnew();
			lpush(tokens, as_token(new_token(strip_nul(line.data, line.size), TOK_DQUOTE_STR, true)));
			tokens = expand_tokens(tokens);
			if (tokens == NULL)
			{
				close(new_hd_fd);
				close(fd);
				unlink(new_hd);
				unlink(file_path);
				simple_command->error = 1;
				return ;
			}
			ft_dprintf(new_hd_fd, "%s", tokens->first->as_token->str);
		}
		close(new_hd_fd);
		close(fd);
		new_hd_fd = open(new_hd, O_RDONLY);
		(void)unlink(new_hd);
		(void)unlink(file_path);
		if (new_hd_fd == -1)
		{
			simple_command->error = minishell_error(1, false, false,
					"%s: %s", file_path, strerror(errno));
			return ;
		}
		sc_fd_in = simple_command->fd_in;
		if (sc_fd_in != -2)
			close(sc_fd_in);
		simple_command->fd_in = new_hd_fd;
		simple_command->error = 0;
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
