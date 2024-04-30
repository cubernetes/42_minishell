#include "../../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>

/*fd_in, fd_out, fd_err*/
/*ft_dprintf(fd, normale printf*/
int	builtin_cd(char **argv, t_fds fds)
{
	char	*var;
	int	status;
	var = getenv("HOME");
	if (!var)
		return (minishell_error(1, FALSE, "cd: HOME not set"));
	if(!*argv)
		status = chdir(var);
	else
	{
		status = chdir(argv[0]);
	}
	if (status == -1)
		return (minishell_error(1, FALSE, "cd: no such file or directory: %s", argv[0]));
	return (0);
}
/* TODO: (not required) implement cd - */
//cd(argv[1]);
//OLDPWD = PWD
//PWD = argv[1]
