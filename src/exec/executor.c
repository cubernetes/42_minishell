/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:30:54 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 22:40:44 by pgrussin         ###   ########.fr       */
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

static void	heredoc_fd_init(t_heredoc_fd *heredoc_fd, char *file_path,
	char *new_hd, t_tree *simple_command)
{
	heredoc_fd->fd = open(file_path, O_RDONLY);
	heredoc_fd->new_hd_fd = open(new_hd, O_CREAT | O_TRUNC | O_RDWR, 0600);
	heredoc_fd->sc_fd_in = simple_command->fd_in;
}

static void	close_all(t_heredoc_fd *heredoc_fd, char **new_hd, char **file_path,
t_tree **simple_command)
{
	if (heredoc_fd->fd != -1)
		close(heredoc_fd->fd);
	if (heredoc_fd->new_hd_fd != -1)
		close(heredoc_fd->new_hd_fd);
	(*simple_command)->error = minishell_error(1, 0,
			"%s: %s", file_path, strerror(errno));
	(void)unlink(*new_hd);
	(void)unlink(*file_path);
}

static void	heredoc_while(t_heredoc_fd *heredoc_fd,
	char **file_path, char **new_hd, t_tree **simple_command)
{
	t_fatptr		line;
	t_list			*tokens;

	while (1)
	{
		line = get_next_fat_line(heredoc_fd->fd);
		if (!line.data)
			break ;
		tokens = lnew();
		lpush(tokens, as_token(new_token(strip_nul(line.data, line.size),
					TOK_DQUOTE_STR, true)));
		tokens = expand_tokens(tokens);
		if (tokens == NULL)
		{
			close(heredoc_fd->new_hd_fd);
			close(heredoc_fd->fd);
			unlink(*new_hd);
			unlink(*file_path);
			(*simple_command)->error = 1;
			return ;
		}
		ft_dprintf(heredoc_fd->new_hd_fd, "%s", tokens->first->as_token->str);
	}
}

static	void	heredoc_close_unlink(t_heredoc_fd *heredoc_fd,
	char **new_hd, char **file_path)
{
	close(heredoc_fd->new_hd_fd);
	close(heredoc_fd->fd);
	heredoc_fd->new_hd_fd = open(*new_hd, O_RDONLY);
	(void)unlink(*new_hd);
	(void)unlink(*file_path);
}

void	redirect_heredoc(char *file_path, t_tree *simple_command)
{
	t_heredoc_fd	heredoc_fd;
	char			*new_hd;

	new_hd = ft_mktemp("minishell.");
	heredoc_fd_init(&heredoc_fd, file_path, new_hd, simple_command);
	if (heredoc_fd.fd == -1 || heredoc_fd.new_hd_fd == -1)
		close_all(&heredoc_fd, &new_hd, &file_path, &simple_command);
	else
	{
		heredoc_while(&heredoc_fd, &file_path, &new_hd, &simple_command);
		heredoc_close_unlink(&heredoc_fd, &new_hd, &file_path);
		if (heredoc_fd.new_hd_fd == -1)
		{
			simple_command->error = minishell_error(1, 0,
					"%s: %s", file_path, strerror(errno));
			return ;
		}
		if (heredoc_fd.sc_fd_in != -2)
			close(heredoc_fd.sc_fd_in);
		simple_command->fd_in = heredoc_fd.new_hd_fd;
		simple_command->error = 0;
	}
}
