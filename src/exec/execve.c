/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:17 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 22:32:53 by tischmid         ###   ########.fr       */
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

void	close_fds(t_tree *command)
{
	int	in;
	int	out;
	int	err;

	in = command->fd_in;
	out = command->fd_out;
	err = command->fd_err;
	if (in != -2)
		close(in);
	if (out != -2)
		close(out);
	if (err != -2)
		close(err);
	command->fd_in = -2;
	command->fd_out = -2;
	command->fd_err = -2;
}

/* TODO: Replace magic number -2 with something like FD_UNINITIALIZED, ...*/
void	set_fds(t_tree *command)
{
	int	in;
	int	out;
	int	err;

	in = command->fd_in;
	out = command->fd_out;
	err = command->fd_err;
	if (in != -2)
		dup2(in, STDIN_FILENO);
	if (out != -2)
		dup2(out, STDOUT_FILENO);
	if (err != -2)
		dup2(err, STDERR_FILENO);
}

char	*handle_path_parts(t_list *path_parts, char *program,
	char *executable_path)
{
	int	fd;

	liter(path_parts);
	while (lnext(path_parts))
	{
		if (*path_parts->current->as_str == '\0')
			path_parts->current->as_str = ".";
		executable_path = ft_strjoin(
				ft_strjoin(path_parts->current->as_str, "/"), program);
		fd = open(executable_path, O_DIRECTORY);
		if (!access(executable_path, X_OK)
			&& fd == -1)
			break ;
		executable_path = NULL;
		close(fd);
	}
	close(fd);
	return (executable_path);
}

char	*search_executable(char *program, t_list *path_parts)
{
	char	*executable_path;

	if (!program || !*program)
		return (NULL);
	if (ft_strchr(program, '/'))
		return (program);
	if (path_parts == NULL)
	{
		if (!ft_strcmp(program, ".."))
			return (program);
		return (NULL);
	}
	executable_path = handle_path_parts(path_parts, program, executable_path);
	return (executable_path);
}

char	**make_argv(t_tree *simple_command)
{
	char	**argv;
	t_list	*d_argv;
	int		i;

	d_argv = lnew();
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
	return (argv);
}
	// if (i > 0)
	// 	set_var("_", argv[i - 1], (t_flags){0});

void	close_other_command_fds(t_list *commands)
{
	liter(commands);
	while (lnext(commands))
		close_fds(commands->current->as_tree);
}
