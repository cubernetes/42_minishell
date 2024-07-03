#include "libft.h"
#include "minishell.h"

int	builtin_readonly(char **argv, t_fds fds)
{
	char	**args;
	int		i;

	if (argv[1] == NULL)
		return (builtin_declare((char *[]){"declare", "-pr", NULL}, fds));
	args = ft_malloc(sizeof(*args) * (3 + ft_arrlen(argv)));
	args[0] = "readonly";
	i = 0;
	if (ft_strcmp(argv[i], "--"))
		++argv;
	while (argv[++i])
		args[i] = argv[i];
	args[i] = NULL;
	return (builtin_declare(args, fds));
}
