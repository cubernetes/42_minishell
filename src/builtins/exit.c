#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

int	builtin_exit(char **argv, t_fds fds)
{
	finish();
	exit(0);
	return (0);
}
