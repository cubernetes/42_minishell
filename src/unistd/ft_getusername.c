/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getusername.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:50:57 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:53:54 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/* TODO: Remove unnecessary headers */
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

static char	*get_username_from_etc_passwd(char *uid)
{
	int		fd;
	char	*line;

	fd = open("/etc/passwd", O_RDONLY);
	if (fd < 0)
		return (var_lookup("USER"));
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (ft_strnstr(line, ft_strjoin("x:", ft_strjoin(uid, ":")),
				ft_strlen(line)) || ft_strnstr(line, ft_strjoin("*:",
					ft_strjoin(uid, ":")), ft_strlen(line)))
			return (close(fd), lsplit(line, ":")->first->as_str);
	}
	close(fd);
	return ("");
}

static int	parse_header(int fd)
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

static int	parse_till_real(int fd)
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

static char	*get_krb_ticket_file(char *uid)
{
	DIR				*dirp;
	struct dirent	*dp;
	char			*name;
	size_t			len;
	char			*big;
	char			*uid_u;
	char			*little;

	if (access("/tmp", F_OK))
		return ("");
	dirp = opendir("/tmp");
	dp = readdir(dirp);
	while (dp != NULL)
	{
		len = ft_strlen(dp->d_name);
		big = ft_strnstr(dp->d_name, "krb5cc_", len);
		uid_u = ft_strjoin(uid, "_");
		little = ft_strjoin("_", uid_u);
		if (big)
			len = ft_strlen(big);
		if (big && ft_strnstr(big, little, len))
			return (name = ft_strdup(dp->d_name), closedir(dirp), ft_strjoin("/tmp/", name));
		dp = readdir(dirp);
	}
	closedir(dirp);
	return ("");
}

static char	*get_username_from_krb_file(int fd)
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

char	*ft_getusername(void)
{
	int		fd;
	char	*uid;
	char	*username;

	username = "";
	uid = ft_getuid();
	if (*uid == '\0')
		username = var_lookup("USER");
	else
	{
		fd = open(get_krb_ticket_file(uid), O_RDONLY);
		if (fd < 0)
			username = get_username_from_etc_passwd(uid);
		else
		{
			username = get_username_from_krb_file(fd);
			close(fd);
		}
	}
	if (*username == '\0')
	{
		if (*uid)
			return (ft_strjoin("u0_a", uid));
		return ("u0_anon");
	}
	return (username);
}
