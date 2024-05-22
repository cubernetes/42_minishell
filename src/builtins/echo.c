#include "minishell.h"

#include <unistd.h>
#include <stdbool.h>

int	builtin_echo(char **argv, t_fds fds)
{
	int		i;
	bool	flag;
	int		j;

	++argv;
	flag = false;
	i = 0;
	j = 0;
	if (argv[0] != NULL && ft_strcmp(argv[0], "-n") == 0)
	{
		flag = true;
		i++;
		j++;
	}
	while (argv[i] != NULL)
	{
		if (fds.fd_out != -2)
		{
			if (i > j)
				ft_dprintf(fds.fd_out, " ");
			ft_dprintf(fds.fd_out, "%s", argv[i++]);
		}
		else
		{
			if (i > j)
				ft_printf(" ");
			ft_printf("%s", argv[i++]);
		}
	}
	if (fds.fd_out != -2)
	{
		if (flag == false)
			ft_dprintf(fds.fd_out, "\n");
		close(fds.fd_out);
	}
	else
	{
		if (flag == false)
			ft_printf("\n");
	}
	return (0);
}
