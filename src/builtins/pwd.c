#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	builtin_pwd(char **argv, t_fds fds)
{
	char	*tmp;

	tmp = gc_add_str(getcwd(NULL, 0));
	printf("%s\n", tmp);
	return (0);
}
