/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gethostname.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:50:10 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 17:54:29 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <fcntl.h>
#include <unistd.h>

char	*ft_gethostname_from_proc(void)
{
	int		fd;
	char	*line;
	size_t	len;

	fd = open("/proc/sys/kernel/hostname", O_RDONLY);
	if (fd < 0)
	{
		if (get_var("HOSTNAME"))
			return (var_lookup("HOSTNAME"));
		return ("localhost");
	}
	line = get_next_line(fd);
	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		line[len - 1] = 0;
	close(fd);
	get_next_line(fd);
	if (line == NULL)
		return ("localhost");
	return (line);
}

/** Return the first line from the file "/etc/hostname".
 *
 *  @returns: the first line from "/etc/hostname";
 *            "" if it fails
 */
char	*ft_gethostname(void)
{
	int		fd;
	char	*line;
	size_t	len;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (ft_gethostname_from_proc());
	line = get_next_line(fd);
	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		line[len - 1] = 0;
	close(fd);
	get_next_line(fd);
	if (line == NULL)
		return ("localhost");
	return (line);
}
