#include "../../include/minishell.h"
#include <unistd.h>
#include <stdio.h>




int	builtin_pwd(char **argv, t_fds fds)
{
	char	*tmp;

	tmp = gc_add(getcwd(NULL, 0))->head->prev->as_str;
	printf("%s\n", tmp);
	return (0);
}
