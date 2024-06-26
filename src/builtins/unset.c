#include "minishell.h"
#include "libft.h"

#include <unistd.h>

int	builtin_unset(char **argv, t_fds fds)
{
	int		exit_status;
	char	*name;
	char	erropt;

	(void)fds;
	ft_getopt(argv, "", &erropt, &optind);
	name = *argv++;
	if (erropt)
		return (minishell_error(2, true, false, "%s: -%c: invalid option\n%s: usage: unset [name ...]", name, erropt, name));
	exit_status = 0;
	while (*argv)
	{
		if (!unset_var(*argv))
			exit_status = minishell_error(1, false, false,
					"%s: %s: cannot unset: readonly variable",
					name, *argv);
		++argv;
	}
	return (exit_status);
}
