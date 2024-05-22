#include "minishell.h"

int	builtin_unset(char **argv, t_fds fds)
{
	(void)fds;
	while (*argv)
	{
		set_var(*argv, NULL, (t_flags){0});
		++argv;
	}
	return (0);
}
