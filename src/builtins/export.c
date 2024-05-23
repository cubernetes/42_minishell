#include "libft.h"
#include "minishell.h"

int	builtin_export(char **argv, t_fds fds)
{
	char	**args;
	int		i;

	if (argv[1] == NULL)
		return (builtin_declare((char *[]){"declare", "-px", NULL}, fds));
	args = ft_malloc(sizeof(*args) * (3 + ft_arrlen(argv)));
	args[0] = "export";
	args[1] = "-x";
	args[2] = "--";
	i = 0;
	while (argv[++i])
		args[2 + i] = argv[i];
	args[2 + i] = NULL;
	return (builtin_declare(args, fds));
}
