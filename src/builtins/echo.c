#include "../../include/minishell.h"
#include <unistd.h>

int	builtin_echo(char **argv, t_fds fds)
{
	int		i;

	// TODO: implement -n when first argument
	i = 0;
	while(argv[i] != NULL)
	{
		if (i > 0)
			ft_dprintf(fds.fd_out, " ");
		if (fds.fd_out != -2)
			ft_dprintf(fds.fd_out, "%s", argv[i++]);
		else
			ft_printf("%s", argv[i++]);
	}
	if (fds.fd_out != -2)
	{
		ft_dprintf(fds.fd_out, "\n");
		close(fds.fd_out);
	}
	else
		ft_printf("\n");
	return (0);
}
