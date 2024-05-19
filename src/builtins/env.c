#include "minishell.h"

int	builtin_env(char **argv, t_fds fds, const char *prefix, bool sort)
{
	char	**env;
	int		i;

	(void)sort;
	(void)argv;
	(void)fds;
	i = 0;
	env = get_env();
	while (env[i])
	{
		ft_printf("%s%s\n", prefix, env[i++]);
	}
	return (0);
}
