#include "../../include/minishell.h"
#include <stdlib.h>

int	builtin_exit(char **argv, t_fds fds)
{
	/*TODO: all freed?*/
	(void)gc_free();
	rl_clear_history();
	exit(0);
	return (0);
}
