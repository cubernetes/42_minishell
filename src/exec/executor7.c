/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 22:19:58 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 22:22:12 by pgrussin         ###   ########.fr       */
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

void	handle_redirect_input(char *file_path, t_tree *simple_command)
{
	int	fd;
	int	sc_fd_in;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		simple_command->error = minishell_error(1, 0,
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
