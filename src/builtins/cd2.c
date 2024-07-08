/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:43 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:31:44 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"
#include "minishell.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int	builtin_cd3(int status, char *cwd, char **argv, char *name)
{
	char	*real_cwd;

	real_cwd = getcwd(NULL, 0);
	if (real_cwd == NULL)
	{
		//TODO: changed message from "!%s: error retrieving current directory: getcwd: cannot access parent directories: %s" to "error"
		minishell_error(0, false, false, "!%s: error: %s",
			name, strerror(errno));
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

static int	builtin_cd2(char **argv, t_fds fds, char *name)
{
	if (!ft_strcmp(*argv, "-"))
	{
		if (get_var("OLDPWD") == NULL)
			return (minishell_error(1, false, false,
					"%s: OLDPWD no set", name));
		*argv = get_var("OLDPWD")->value;
		argv[1] = "-";
		ft_dprintf(fds.fd_out, "%s\n", get_var("OLDPWD")->value);
		return (chdir(get_var("OLDPWD")->value));
	}
	else if (**argv == '\0')
		return (chdir("."));
	else
		return (chdir(*argv));
}

int	builtin_cd(char **argv, t_fds fds)
{
	char	*var;
	int		status;
	char	*name;
	char	*cwd;

	name = *argv++;
	cwd = NULL;
	if (argv[1] != NULL)
	//TODO: changed from "%s: too many arguments" to "w"
		return (minishell_error(1, false, false, "%s: w", name));
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
	status = builtin_cd2(argv, fds, name);
	if (status != 0)
		return (minishell_error(1, false, false, "%s: %s: %s",
				name, *argv, strerror(errno)));
	else
		return (builtin_cd3(status, cwd, argv, name));
}
/* TODO: cd with null argument is same as cd . */
