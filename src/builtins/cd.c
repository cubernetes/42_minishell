#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static char	*normalize(char *path)
{
	t_list	*parts;
	t_list	*new_parts;

	parts = liter(lsplit(path, "/"));
	new_parts = lnew();
	while (lnext(parts))
	{
		if (!ft_strcmp(parts->current->as_str, ""));
		else if (!ft_strcmp(parts->current->as_str, "."));
		else if (!ft_strcmp(parts->current->as_str, ".."))
			lpop(new_parts);
		else
			lpush(new_parts, as_data(parts->current));
	}
	return (ft_strjoin("/", ljoin(new_parts, "/")));
}

int	builtin_cd(char **argv, t_fds fds)
{
	char	*var;
	int		status;
	char	*name;
	char	*pwd;

	name = *argv++;
	if (*argv == NULL)
	{
		var = NULL;
		if (get_var("HOME") != NULL)
			var = get_var("HOME")->value;
		if (!var)
			return (minishell_error(1, false, "%s: HOME not set", name));
		status = chdir(var);
	}
	else if (!ft_strcmp(*argv, "-"))
	{
		if (get_var("OLDPWD") == NULL)
			return (minishell_error(1, false, "%s: OLDPWD no set", name));
		*argv = get_var("OLDPWD")->value;
		argv[1] = "-";
		ft_dprintf(fds.fd_out, "%s\n", get_var("OLDPWD")->value);
		status = chdir(get_var("OLDPWD")->value);
	}
	else
		status = chdir(*argv);
	if (status == -1)
		return (minishell_error(1, false, "%s: no such file or directory: %s", name, *argv));
	else
	{
		if (get_var("PWD") == NULL)
			pwd = gc_add_str(getcwd(NULL, 0));
		else
			pwd = get_var("PWD")->value;
		set_var("OLDPWD", pwd, get_flags("OLDPWD"));
		if (**argv == '/' || (argv[1] && !ft_strcmp(argv[1], "-")))
			set_var("PWD", normalize(*argv), get_flags("PWD"));
		else
			set_var("PWD", normalize(ft_strjoin(pwd, ft_strjoin("/", *argv))), get_flags("PWD"));
	}
	return (0);
}
