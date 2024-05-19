#include "minishell.h"

int	builtin_env(char **argv, t_fds fds)
{
	char	**env;
	int		i;
	
	(void)argv;
	(void)fds;
	i = 0;
	env = get_env();
	while (env[i])
	{
		ft_printf("%s\n", env[i++]);
	}
	return (0);
}
