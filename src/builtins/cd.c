#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static char	*normalize(char *path)
{
	char	*new_path;
	int		i;

	path = ft_strjoin(path, "/");
	new_path = "";
	i = 0;
	while (path[i])
	{
		if (path[i] == '/' && path[i + 1] == '/')
		{
			new_path = ft_strjoin(new_path, "/");
			if (path[++i] == '\0')
				break ;
		}
		else
			new_path = ft_strjoin(new_path, ft_strndup(&path[i], 1))
		++i;
	}
    // ft_replace_all(new_path, "/..", "/");
	return (new_path);
}

/*fd_in, fd_out, fd_err*/
/*ft_dprintf(fd, normale printf*/
int	builtin_cd(char **argv, t_fds fds)
{
	char	*var;
	int		status;
	char	*name;
	char	*pwd;

	name = *argv++;
	var = NULL;
	if (get_var("HOME") != NULL)
		var = get_var("HOME")->value;
	if (!var)
		return (minishell_error(1, false, "%s: HOME not set", name));
	if (!*argv)
		status = chdir(var);
	else if (!ft_strcmp(*argv, "-"))
	{
		if (get_var("OLDPWD") == NULL)
			return (minishell_error(1, false, "%s: OLDPWD no set", name));
		status = chdir(get_var("OLDPWD")->value);
	}
	else
		status = chdir(argv[0]);
	if (status == -1)
		return (minishell_error(1, false, "%s: no such file or directory: %s", name, argv[0]));
	else
	{
		if (get_var("PWD") == NULL)
			pwd = gc_add_str(getcwd(NULL, 0));
		else
			pwd = get_var("PWD")->value;
		set_var("OLDPWD", pwd, (t_flags){0});
		set_var("PWD", argv[0], (t_flags){0});
	}
	return (0);
}
