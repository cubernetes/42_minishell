/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_username_from_krb.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:03:09 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 17:54:46 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft.h"

#include <unistd.h>
#include <dirent.h>

int	parse_header(int fd)
{
	char	version;
	short	length;
	char	*buffer;

	if (read(fd, &version, 1) < 1)
		return (1);
	if (read(fd, &version, 1) < 1)
		return (2);
	if (version != 4)
		return (3);
	if (read(fd, (char *)&length + 1, 1) < 1)
		return (4);
	if (read(fd, (char *)&length + 0, 1) < 1)
		return (5);
	buffer = ft_malloc(sizeof(char) * (size_t)length);
	if (read(fd, buffer, (size_t)length) < length)
		return (6);
	return (0);
}

int	parse_till_real(int fd)
{
	int		length;
	char	*buffer;

	if (read(fd, &length, 4) < 4)
		return (1);
	if (read(fd, &length, 4) < 4)
		return (2);
	if (read(fd, (char *)&length + 3, 1) < 1)
		return (3);
	if (read(fd, (char *)&length + 2, 1) < 1)
		return (4);
	if (read(fd, (char *)&length + 1, 1) < 1)
		return (5);
	if (read(fd, (char *)&length + 0, 1) < 1)
		return (6);
	buffer = ft_malloc(sizeof(char) * (size_t)length);
	if (read(fd, buffer, (size_t)length) < length)
		return (7);
	return (0);
}

char	*get_username_from_krb_file(int fd)
{
	int		length;
	char	*buffer;

	if (parse_header(fd))
		return (close(fd), "");
	if (parse_till_real(fd))
		return (close(fd), "");
	if (read(fd, (char *)&length + 3, 1) < 1)
		return (close(fd), "");
	if (read(fd, (char *)&length + 2, 1) < 1)
		return (close(fd), "");
	if (read(fd, (char *)&length + 1, 1) < 1)
		return (close(fd), "");
	if (read(fd, (char *)&length + 0, 1) < 1)
		return (close(fd), "");
	buffer = ft_malloc(sizeof(char) * (size_t)(length + 1));
	buffer[length] = 0;
	if (read(fd, buffer, (size_t)length) < length)
		return (close(fd), "");
	return (close(fd), buffer);
}

char	*get_krb_ticket_file(char *uid)
{
	DIR				*dirp;
	struct dirent	*dp;
	size_t			len;
	char			*big;
	char			*tmp;

	if (access("/tmp", F_OK))
		return ("");
	dirp = opendir("/tmp");
	dp = readdir(dirp);
	while (dp != NULL)
	{
		len = ft_strlen(dp->d_name);
		big = ft_strnstr(dp->d_name, "krb5cc_", len);
		tmp = ft_strjoin(uid, "_");
		tmp = ft_strjoin("_", tmp);
		if (big)
			len = ft_strlen(big);
		if (big && ft_strnstr(big, tmp, len))
			return (tmp = ft_strdup(dp->d_name), closedir(dirp),
				ft_strjoin("/tmp/", tmp));
		dp = readdir(dirp);
	}
	closedir(dirp);
	return ("");
}
