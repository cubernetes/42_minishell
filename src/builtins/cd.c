#include "libft.h"
#include "minishell.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

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

char	*set_saved_cwd(char *cwd)
{
	static char	*_cwd;

	if (cwd != NULL)
	{
		gc_start_context("POST");
		_cwd = ft_strdup(cwd);
		gc_end_context();
	}
	return (_cwd);
}

char	*get_saved_cwd(void)
{
	return (set_saved_cwd(NULL));
}

int	builtin_cd(char **argv, t_fds fds)
{
	char	*var;
	int		status;
	char	*name;
	char	*cwd;
	char	*real_cwd;

	name = *argv++;
	if (argv[1] != NULL)
		return (minishell_error(1, false, false, "%s: too many arguments", name));
	if (*argv == NULL)
	{
		var = NULL;
		if (get_var("HOME") != NULL)
			var = get_var("HOME")->value;
		if (!var)
			return (minishell_error(1, false, false, "%s: HOME not set", name));
		*argv = var;
		status = chdir(var);
	}
	else if (!ft_strcmp(*argv, "-"))
	{
		if (get_var("OLDPWD") == NULL)
			return (minishell_error(1, false, false, "%s: OLDPWD no set", name));
		*argv = get_var("OLDPWD")->value;
		argv[1] = "-";
		ft_dprintf(fds.fd_out, "%s\n", get_var("OLDPWD")->value);
		status = chdir(get_var("OLDPWD")->value);
	}
	else if (**argv == '\0')
		status = chdir(".");
	else
		status = chdir(*argv);

	if (status != 0)
		return (minishell_error(1, false, false, "%s: %s: %s", name, *argv, strerror(errno)));
	else
	{
		real_cwd = getcwd(NULL, 0);
		if (real_cwd == NULL)
		{
			minishell_error(0, false, false, "!%s: error retrieving current directory: getcwd: cannot access parent directories: %s", name, strerror(errno));
			status = 1;
		}
		cwd = get_saved_cwd();
		set_var("OLDPWD", cwd, get_flags("OLDPWD"));
		if (**argv == '/' || (argv[1] && !ft_strcmp(argv[1], "-")))
			cwd = normalize(*argv);
		else if (status == 0)
			cwd = normalize(ft_strjoin(cwd, ft_strjoin("/", *argv)));
		else if (access(cwd, F_OK))
			cwd = ft_strjoin(cwd, ft_strjoin("/", *argv));
		else
			cwd = *argv;
		set_saved_cwd(cwd);
		set_var("PWD", cwd, get_flags("PWD"));
		return (0);
	}
}
/* TODO: cd with null argument is same as cd . */
