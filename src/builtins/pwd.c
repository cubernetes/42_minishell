#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	builtin_pwd(char **argv, t_fds fds)
{
	ft_dprintf(fds.fd_out, "%s\n", gc_add_str(getcwd(NULL, 0)));
	return (0);
}
