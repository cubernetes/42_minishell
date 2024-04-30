#include "../../include/minishell.h"

int	builtin_echo(char **argv, t_fds fds)
{
	int		i;

	i = 0;
	while(argv[i] != NULL)
	{
		if (i > 0)
			ft_dprintf(fds.fd_out, " ");
		ft_dprintf(fds.fd_out, "%s", argv[i++]);
	}
	ft_dprintf(fds.fd_out, "\n");
	close(fds.fd_out);
	return (0);
}
