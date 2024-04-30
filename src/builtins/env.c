#include "../../include/minishell.h"

int	builtin_env(char **argv, t_fds fds)
{
	char	**env;
	int		i;
	
	i = 0;
	env = get_env();
	while (env[i])
	{
		printf("%s\n", env[i++]);
	}
	return (0);
}
