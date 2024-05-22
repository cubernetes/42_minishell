#include "minishell.h"

int	builtin_unset(char **argv, t_fds fds)
{
	int		exit_status;
	char	*name;

	name = *argv++;
	exit_status = 0;
	while (*argv)
	{
		if (!unset_var(*argv))
			exit_status = minishell_error(1, false, "%s: %s: cannot unset: readonly variable",
				name, *argv);
		++argv;
	}
	return (exit_status);
}
