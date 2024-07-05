/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getusername.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:50:57 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 02:06:07 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft.h"

#include <unistd.h>
#include <fcntl.h>

char	*get_krb_ticket_file(char *uid);
char	*get_username_from_krb_file(int fd);

static char	*_get_username_from_etc_passwd(char *uid)
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
			username = _get_username_from_etc_passwd(uid);
		else
		{
			username = get_username_from_krb_file(fd);
			close(fd);
		}
	}
	if (*username == '\0' && *uid)
		return (ft_strjoin("u0_a", uid));
	else if (*username == '\0')
		return ("u0_anon");
	return (username);
}
