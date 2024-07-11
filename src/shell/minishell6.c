/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:00:43 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/11 15:34:38 by tosuman          ###   ########.fr       */
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

static int	noop(void)
{
	return (0);
}

int	mk_err_flags(bool do_exit, bool syntax_error)
{
	int	res;

	res = 0;
	if (do_exit)
		res |= DO_EXIT;
	if (syntax_error)
		res |= SYNTAX_ERROR;
	return (res);
}

static void	read_init_files(bool is_login_shell)
{
	char	*profile;
	char	*rc;

	profile = ft_strjoin(var_lookup("HOME"), "/.msh_profile");
	rc = ft_strjoin(var_lookup("HOME"), "/.mshrc");
	if (is_login_shell)
	{
		if (!access(profile, R_OK) && open(profile, O_DIRECTORY) == -1)
			set_var("MINISHELL_SOURCE_EXECUTION_STRING",
				"source \"$HOME/.msh_profile\"",
				get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
	}
	else if (option_enabled('i'))
		if (!access(rc, R_OK) && open(rc, O_DIRECTORY) == -1)
			set_var("MINISHELL_SOURCE_EXECUTION_STRING",
				"source \"$HOME/.mshrc\"",
				get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
}

void	init(char *argv[], char *envp[])
{
	bool	is_login_shell;

	set_allocator(gc_malloc);
	gc_set_context("DEFAULT");
	set_initial_shell_variables(argv, envp);
	is_login_shell = set_shell_options(argv);
	rl_event_hook = noop;
	rl_outstream = stderr;
	read_init_files(is_login_shell);
}
