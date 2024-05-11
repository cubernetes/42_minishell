#include "libft.h"

#include <fcntl.h>

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
		return ("");
	line = get_next_line(fd);
	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		line[len - 1] = 0;
	close(fd);
	get_next_line(fd);
	if (line == NULL)
		return ("");
	return (line);
}
