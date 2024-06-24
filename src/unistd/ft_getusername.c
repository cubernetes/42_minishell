#include "minishell.h"
#include "libft.h"

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
		if (ft_strnstr(line, ft_strjoin("x:", ft_strjoin(uid, ":")), ft_strlen(line)) || ft_strnstr(line, ft_strjoin("*:", ft_strjoin(uid, ":")), ft_strlen(line)))
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

	if (read(fd, &version, 1) < 1) // 5
		return (1);
	if (read(fd, &version, 1) < 1) // version
		return (2);
	if (version != 4)
		return (3);
	if (read(fd, (char *)&length + 1, 1) < 1) // length of header lsb
		return (4);
	if (read(fd, (char *)&length + 0, 1) < 1) // msb
		return (5);
	buffer = ft_malloc(sizeof(char) * (size_t)length);
	if (read(fd, buffer, (size_t)length) < length) // header
		return (6);
	return (0);
}

static int	parse_till_real(int fd)
{
	int		length;
	char	*buffer;

	if (read(fd, &length, 4) < 4) // name
		return (1);
	if (read(fd, &length, 4) < 4) // count of components -- should be one
		return (2);
	if (read(fd, (char *)&length + 3, 1) < 1) // length of realm
		return (3);
	if (read(fd, (char *)&length + 2, 1) < 1)
		return (4);
	if (read(fd, (char *)&length + 1, 1) < 1)
		return (5);
	if (read(fd, (char *)&length + 0, 1) < 1)
		return (6);
	buffer = ft_malloc(sizeof(char) * (size_t)length);
	if (read(fd, buffer, (size_t)length) < length) // realm
		return (7);
	return (0);
}

/* Return UID (0=real, effective, set saved, 3=filesystem) from /proc/self/status */
char	*ft_get_specific_uid(int uid_type)
{
	char	*line;
	int		fd;
	t_list	*parts;

	fd = open("/proc/self/status", O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (ft_strnstr(line, "Uid:", ft_strlen(line)))
		{
			parts = liter(lsplit(line, "\t"));
			close(fd);
			lrotate(uid_type);
			if (parts->len == 5)
				return (parts->first->next->as_str);
			if (uid_type == 0)
				return (var_lookup("UID"));
			return (var_lookup("EUID"));
		}
	}
	if (uid_type == 0)
		return (close(fd), var_lookup("UID"));
	return (close(fd), var_lookup("EUID"));
}

/* Return real UID from /proc/self/status */
char	*ft_getuid(void)
{
	return ft_get_specific_uid(0);
}

/* Return effective UID from /proc/self/status */
char	*ft_geteuid(void)
{
	return ft_get_specific_uid(1);
}

// static char	*ft_getloginuid(void)
// {
// 	int		fd;
// 	char	buf[BUFSIZ];
// 	ssize_t	bytes_read;
// 
// 	fd = open("/proc/self/loginuid", O_RDONLY);
// 	bytes_read = read(fd, buf, BUFSIZ);
// 	buf[bytes_read] = 0;
// 	close(fd);
// 	if (!ft_strcmp(buf, "4294967295"))
// 		return (var_lookup("EUID"));
// 	return (ft_strdup(buf));
// }

static char	*get_krb_ticket_file(char *uid)
{
	DIR				*dirp;
	struct dirent	*dp;
	char			*name;

	if (access("/tmp", F_OK))
		return ("");
	dirp = opendir("/tmp");
	dp = readdir(dirp);
	while (dp != NULL)
	{
		size_t len = ft_strlen(dp->d_name);
		char *big = ft_strnstr(dp->d_name, "krb5cc_", len);
		char *uid_u = ft_strjoin(uid, "_");
		char *little = ft_strjoin("_", uid_u);
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
	if (read(fd, (char *)&length + 3, 1) < 1) // length of component1
		return (close(fd), "");
	if (read(fd, (char *)&length + 2, 1) < 1)
		return (close(fd), "");
	if (read(fd, (char *)&length + 1, 1) < 1)
		return (close(fd), "");
	if (read(fd, (char *)&length + 0, 1) < 1)
		return (close(fd), "");
	buffer = ft_malloc(sizeof(char) * (size_t)(length + 1));
	buffer[length] = 0;
	if (read(fd, buffer, (size_t)length) < length) // component1
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
