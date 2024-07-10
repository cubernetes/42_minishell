/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:33 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 17:54:00 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "libft.h"
#include "minishell.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

char	*search_file(char *file_path, char **path_parts)
{
	char	*path;
	char	*executable_path;

	if (ft_strchr(file_path, '/'))
		return (file_path);
	executable_path = NULL;
	while (file_path && *file_path && *path_parts)
	{
		path = ft_strjoin(*path_parts, "/");
		executable_path = ft_strjoin(path, file_path);
		if (!access(executable_path, F_OK)
			&& open(executable_path, O_DIRECTORY) == -1)
			break ;
		executable_path = NULL;
		++path_parts;
	}
	return (executable_path);
}

char	*read_file_contents(const char *file_path)
{
	int		fd;
	char	*lines;
	char	*line;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	lines = "";
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		lines = ft_strjoin(lines, line);
	}
	close(fd);
	if (lines[ft_strlen(lines) - 1] == '\n')
		lines[ft_strlen(lines) - 1] = '\0';
	return (lines);
}

int	source_error(char *const argv[])
{
	errno = ENOENT;
	return (minishell_error(1, false, false,
			"%s: %s", argv[1], strerror(errno)));
}

int	builtin_source(char **argv, t_fds fds)
{
	char	**path_parts;
	char	*file_path;
	char	*lines;

	(void)fds;
	if (argv[1] == NULL)
		return (minishell_error(2, false, false, "%s: filename argument "
				"required\n%s: usage: %s filename [arguments]",
				argv[0], argv[0], argv[0]));
	path_parts = ft_split(ft_strjoin(".:", var_lookup("PATH")), ':');
	file_path = search_file(argv[1], path_parts);
	if (!file_path)
		return (source_error(argv));
	else if (open(file_path, O_DIRECTORY) != -1)
		return (minishell_error(1, false, false,
				"%s: %s: is a directory", argv[0],
				lsplit(file_path, "/")->last->as_str));
	lines = read_file_contents(file_path);
	if (lines == NULL)
		return (minishell_error(1, false, false,
				"%s: %s", file_path, strerror(errno)));
	set_var("MINISHELL_SOURCE_EXECUTION_STRING", lines,
		get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
	return (0);
}
