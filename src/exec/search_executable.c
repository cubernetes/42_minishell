/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_executable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 22:46:03 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/11 00:50:46 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

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
		if (fd != -1)
			close(fd);
	}
	if (fd != -1)
		close(fd);
	return (executable_path);
}

char	*search_executable(char *program, t_list *path_parts)
{
	char	*executable_path;

	executable_path = NULL;
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
