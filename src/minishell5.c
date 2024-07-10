/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:48:22 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 23:48:05 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <stddef.h>
#include <stdio.h>             /* needed for FILE typedef needed by readline */
#include <readline/history.h>  /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h>            /* exit(), DONT USE free or malloc! */
#include <string.h>
#include <unistd.h>            /* STDERR_FILENO */
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void	handle_pwd_existing_2(unsigned long cwd_inode, char *real_path,
t_var *cwd_path, struct stat real_cwd_stat)
{
	unsigned long	real_cwd_inode;

	real_cwd_inode = real_cwd_stat.st_ino;
	if (real_cwd_inode != cwd_inode)
	{
		set_var("PWD", real_path, (t_flags){.exp = true});
		set_saved_cwd(real_path);
	}
	else
	{
		if (!access(real_path, X_OK))
		{
			set_var("PWD", cwd_path->value, (t_flags){.exp = true});
			set_saved_cwd(cwd_path->value);
		}
		else
		{
			set_var("PWD", real_path, (t_flags){.exp = true});
			set_saved_cwd(real_path);
		}
	}
}

static void	handle_pwd_existing(t_var *cwd_path, struct stat cwd_stat)
{
	unsigned long	cwd_inode;
	char			*real_path;
	struct stat		real_cwd_stat;

	real_path = gc_add_str(getcwd(NULL, 0));
	cwd_inode = cwd_stat.st_ino;
	if (real_path == NULL)
	{
		minishell_error(0, 0,
			"!shell-init: error retrieving current directory: getcwd: cannot "
			"access parent directory: %s", strerror(errno));
		set_var("PWD", cwd_path->value, (t_flags){.exp = true});
		set_saved_cwd(cwd_path->value);
		return ;
	}
	if (stat(real_path, &real_cwd_stat) < 0)
	{
		minishell_error(0, 0, "!shell-init: error retrieving "
			"current directory: getcwd: cannot access parent directory: "
			"%s", strerror(errno));
		set_var("PWD", cwd_path->value, (t_flags){.exp = true});
		set_saved_cwd(cwd_path->value);
		return ;
	}
	handle_pwd_existing_2(cwd_inode, real_path, cwd_path, real_cwd_stat);
}

void	setup_pwd(void)
{
	t_var		*cwd_path;
	struct stat	cwd_stat;

	cwd_path = get_var("PWD");
	if (cwd_path != NULL)
	{
		if (stat(cwd_path->value, &cwd_stat) < 0)
			handle_pwd_missing();
		else
			handle_pwd_existing(cwd_path, cwd_stat);
	}
	else
		handle_pwd_missing();
}

void	set_initial_shell_variables(char *argv[], char *envp[])
{
	inherit_environment(envp);
	setup_pwd();
	set_argv(argv);
	set_var("?", "0", (t_flags){.special = true});
	unset_var("MINISHELL_XTRACEFD");
	set_var("$", ft_getpid(), (t_flags){.special = true});
	set_var("PPID", ft_getppid(), (t_flags){.readonly = true});
	set_var("LINENO", "0", (t_flags){0});
	set_var("SHLVL", ft_itoa(ft_atoi(var_lookup("SHLVL")) + 1),
		(t_flags){.exp = true});
	set_var("_", argv[0], (t_flags){0});
	set_var("PS0", PS0, (t_flags){0});
	set_var("PS1", "\033[31m\\u\033[m@\033[94m\\h\033[m@\033[92mmsh\033[m "
		"[\033[32m\\w\033[m]\n\\$ ", (t_flags){0});
	set_var("PS2", PS2, (t_flags){0});
	set_var("PS4", PS4, (t_flags){0});
	set_var("IFS", DEFAULT_IFS, (t_flags){0});
	set_var("MINISHELL_SOURCE_EXECUTION_STRING", NULL,
		(t_flags){.readonly = true});
}
// TODO: Ignore: If it has a value and is unset or set to a new value, 
// the fd corresponding to the old value shall be closed.
// TODO: Ignore: SHLVL: Depends on if interactive or not
