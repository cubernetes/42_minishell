#include "minishell.h"
#include "libft.h"

#include <fcntl.h>
#include <unistd.h>

/* TODO: Move to libft */
int	ft_octal_to_int(const char *oct)
{
	int	res;
	int	sign;

	while (ft_isspace(*oct++))
		;
	sign = 1;
	if (*--oct == '+' || *oct == '-')
		if (*oct++ == '-')
			sign = -1;
	res = 0;
	while (ft_strchr("01234567", *oct) && *oct != '\0')
		res = res * 8 + *oct++ - '0';
	return (sign * res);
}

int	ft_getumask(void)
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
		if (!ft_strncmp(line, "Umask:", 6))
		{
			parts = liter(lsplit(line, "\t"));
			close(fd);
			pid = ft_strtrim(parts->first->next->as_str, " \t\n");
			i = -1;
			while (pid[++i])
				if (!ft_isdigit(pid[i]))
					return (ft_octal_to_int("7077"));
			return (ft_octal_to_int(pid));
		}
	}
	return (close(fd), ft_octal_to_int("7077"));
}
