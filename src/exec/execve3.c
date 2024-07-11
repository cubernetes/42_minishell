/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:14:54 by paul              #+#    #+#             */
/*   Updated: 2024/07/11 16:28:28 by tosuman          ###   ########.fr       */
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

void	set_underscore(char *const argv[])
{
	if (*argv == NULL)
		return ;
	while (*argv)
		++argv;
	--argv;
	set_var("_", *argv, (t_flags){0});
}

char	**copy_argv(char *const argv[])
{
	int		i;
	char	**new_argv;

	i = 0;
	while (argv[i])
		++i;
	new_argv = ft_malloc(sizeof(*new_argv) * ((size_t)i + 1));
	i = -1;
	while (argv[++i])
		new_argv[i] = argv[i];
	new_argv[i] = NULL;
	return (new_argv);
}

t_fds	setup_file_descriptors(t_tree *simple_command)
{
	t_fds	orig_fds;

	orig_fds.fd_in = simple_command->fd_in;
	orig_fds.fd_out = simple_command->fd_out;
	orig_fds.fd_err = simple_command->fd_err;
	if (simple_command->fd_in == -2)
		simple_command->fd_in = STDIN_FILENO;
	if (simple_command->fd_out == -2)
		simple_command->fd_out = STDOUT_FILENO;
	if (simple_command->fd_err == -2)
		simple_command->fd_err = STDERR_FILENO;
	return (orig_fds);
}

void	cleanup_file_descriptors(t_tree *simple_command, t_fds orig_fds)
{
	if (orig_fds.fd_in != -2)
		close(orig_fds.fd_in);
	if (orig_fds.fd_out != -2)
		close(orig_fds.fd_out);
	if (orig_fds.fd_err != -2)
		close(orig_fds.fd_err);
	simple_command->fd_in = -2;
	simple_command->fd_out = -2;
	simple_command->fd_err = -2;
}

int	handle_builtin_wrapper(char *argv[], t_tree *simple_command)
{
	t_fds	orig_fds;
	int		exit_status;

	orig_fds = setup_file_descriptors(simple_command);
	exit_status = handle_builtin(copy_argv(argv), simple_command->fds);
	cleanup_file_descriptors(simple_command, orig_fds);
	set_underscore(argv);
	return (exit_status);
}
