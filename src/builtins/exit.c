#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

int	builtin_exit(char **argv, t_fds fds)
{
	long long	exit_status;
	char		*name;
	int			status;

	name = *argv++;
	if (*argv == NULL)
		exit_status = ft_atoi(get_var("?")->value);
	else
	{
		exit_status = ft_atol_status(*argv, &status);
		if (status & 5)
			minishell_error(2, true, "%s: %s: numeric argument required", name, *argv);
		if (argv[1])
		{
			minishell_error(1, false, "%s: too many arguments", name);
			return (1); // TODO: don't set exit status when exit fails
		}
	}
	finish(true);
	exit((unsigned char)exit_status);
	return (0);
}
