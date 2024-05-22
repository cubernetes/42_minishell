#include "minishell.h"

#include <stdlib.h>

int	builtin_exit(char **argv, t_fds fds)
{
	(void)argv;
	(void)fds;
	finish();
	exit(0);
	return (0);
}
