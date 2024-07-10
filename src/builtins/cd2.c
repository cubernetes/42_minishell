/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:43 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 17:54:10 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int	builtin_cd3(int status, char *cwd, char **argv, char *name)
{
	char	*real_cwd;

	real_cwd = getcwd(NULL, 0);
	if (real_cwd == NULL)
	{
		minishell_error(0, false, false, "!%s: error retrieving current "\
				"directory: getcwd: cannot access parent directories: %s",
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

int	builtin_cd2(char **argv, t_fds fds, char *name)
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
	char	*nm;
	char	*cwd;

	nm = *argv++;
	cwd = NULL;
	if (argv[1] != NULL)
		return (minishell_error(1, false, false, "%s: too many arguments", nm));
	if (*argv == NULL)
	{
		var = NULL;
		if (get_var("HOME") != NULL)
			var = get_var("HOME")->value;
		if (!var)
			return (minishell_error(1, false, false, "%s: HOME not set", nm));
		*argv = var;
		status = chdir(var);
	}
	status = builtin_cd2(argv, fds, nm);
	if (status != 0)
		return (minishell_error(1, false, false, "%s: %s: %s",
				nm, *argv, strerror(errno)));
	else
		return (builtin_cd3(status, cwd, argv, nm));
}
/* TODO: cd with null argument is same as cd . */
