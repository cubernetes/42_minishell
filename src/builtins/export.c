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
	i = 0;
	if (!ft_strcmp(argv[i + 1], "--"))
		++argv;
	while (argv[++i])
		args[i] = argv[i];
	args[i] = NULL;
	return (builtin_declare(args, fds));
}
