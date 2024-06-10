#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

int	builtin_exit(char **argv, t_fds fds)
{
	int	exit_status;

	exit_status = ft_atoi(get_var("?")->value);
	finish();
	exit(exit_status);
	return (0);
}
