#include "minishell.h"

int	builtin_exit(char **argv, t_fds fds)
{
	finish();
	exit(0);
	return (0);
}
