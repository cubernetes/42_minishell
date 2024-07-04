/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getpid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:50:25 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:50:26 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <fcntl.h>
#include <unistd.h>

char	*ft_getpid(void)
{
	char	*line;
	int		fd;
	t_list	*parts;
	char	*pid;
	int		i;

	fd = open("/proc/self/stat", O_RDONLY);
	line = get_next_line(fd);
	close(fd);
	if (!line)
		return ("0");
	parts = liter(lsplit(line, " "));
	if (parts->len < 1)
		return ("0");
	pid = ft_strtrim(parts->first->as_str, " \t\n");
	i = -1;
	while (pid[++i])
		if (!ft_isdigit(pid[i]))
			return ("0");
	return (pid);
}

char	*ft_getppid(void)
{
	char	*line;
	int		fd;
	t_list	*parts;
	int		i;
	char	*pid;

	fd = open("/proc/self/status", O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (!ft_strncmp(line, "PPid:", 5))
		{
			parts = liter(lsplit(line, "\t"));
			close(fd);
			pid = ft_strtrim(parts->first->next->as_str, " \t\n");
			i = -1;
			while (pid[++i])
				if (!ft_isdigit(pid[i]))
					return ("0");
			return (pid);
		}
	}
	return (close(fd), "0");
}
