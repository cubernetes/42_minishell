/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:48:22 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 18:00:31 by pgrussin         ###   ########.fr       */
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
		minishell_error(0, false, false,
			"!shell-init: error retrieving current directory: getcwd: cannot access parent directory: %s", strerror(errno));
		set_var("PWD", cwd_path->value, (t_flags){.exp = true});
		set_saved_cwd(cwd_path->value);
		return ;
	}
	if (stat(real_path, &real_cwd_stat) < 0)
	{
		minishell_error(0, false, false, "!shell-init: error retrieving current directory: getcwd: cannot access parent directory: %s", strerror(errno));
		set_var("PWD", cwd_path->value, (t_flags){.exp = true});
		set_saved_cwd(cwd_path->value);
		return ;
	}
	handle_pwd_existing_2(cwd_inode, real_path, cwd_path, real_cwd_stat);
}

void	setup_pwd(void)
{
	t_var		*cwd_path;
	/*TODO: was stat?*/
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

/* TODO: make logic correct */
void	set_pwd(void)
{
	t_var	*pwd_var;

	pwd_var = get_var("PWD");
	if (pwd_var == NULL)
	{
		pwd_var = ft_malloc(sizeof(*pwd_var));
		pwd_var->value = NULL;
	}
	if (pwd_var->value == NULL)
	{
		pwd_var->value = getcwd(NULL, 0);
		gc_add(pwd_var->value);
	}
	set_var("PWD", pwd_var->value, (t_flags){.exp = true});
}
/* On startup, bash sets the value of PWD to getcwd(2) when it is unset.
 * However, when it is set already (through inheritance), then it is not
 * updated until the next chdir(2) UNLESS the directory described by PWD
 * does not exist, does not refer to the same inode number as the directory
 * described by getcwd(2), in which case it is set to getcwd(2).
 */

void	set_initial_shell_variables(char *argv[], char *envp[])
{
	inherit_environment(envp);
	setup_pwd();
	set_argv(argv);
	set_var("?", "0", (t_flags){.special = true});
	unset_var("MINISHELL_XTRACEFD"); // TODO: If it has a value and is unset or set to a new value, the fd corresponding to the old value shall be closed.
	set_var("$", ft_getpid(), (t_flags){.special = true});
	set_var("PPID", ft_getppid(), (t_flags){.readonly = true});
	set_var("LINENO", "0", (t_flags){0});
	// TODO: Depends on if interactive or not
	set_var("SHLVL", ft_itoa(ft_atoi(var_lookup("SHLVL")) + 1),
		(t_flags){.exp = true});
	set_var("_", argv[0], (t_flags){0});
	set_var("PS0", PS0, (t_flags){0});
	set_var("PS1", PS1, (t_flags){0});
	set_var("PS2", PS2, (t_flags){0});
	set_var("PS4", PS4, (t_flags){0});
	set_var("IFS", DEFAULT_IFS, (t_flags){0});
	set_var("MINISHELL_SOURCE_EXECUTION_STRING", NULL,
		(t_flags){.readonly = true});
	/* set_pwd(); */
}
