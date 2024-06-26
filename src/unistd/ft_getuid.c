#include "minishell.h"
#include "libft.h"

#include <fcntl.h>
#include <unistd.h>

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
		if (!ft_strncmp(line, "Uid:", 4))
		{
			parts = liter(lsplit(line, "\t"));
			close(fd);
			lrotate(parts, 1);
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
	return (ft_get_specific_uid(0));
}

/* Return effective UID from /proc/self/status */
char	*ft_geteuid(void)
{
	return (ft_get_specific_uid(1));
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
