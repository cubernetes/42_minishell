/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:00:43 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 22:47:25 by tischmid         ###   ########.fr       */
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

/* TODO: remove DEBUG macros */
/* TODO: remove deque_print(tokens, print_token); */
/* TODO: glob expansion, env var expansion, and quote handling */
/* TODO: protect all mallocs (e.g. xmalloc(size_t n, int lineno), etc.) */
/* TODO: check for NULL when using any function that returns a malloced ptr */
/* TODO: make functions static where needed */
/* TODO: check for forbidden functions (backtrace): nm minishell */
/* TODO: check all gc_free() calls */
/* TODO: make gc_free() callable multiple times */
/* TODO: find unused functions (like printing functions) */
/* TODO: write test functions for deque */
/* TODO: use -MMD and stuff */
/* TODO: revise all minishell_error() calls */
/* TODO: remove unnecessary indentation in variables declarations */
/* TODO: use ptr[static 1] where needed */
/* TOOD: Not required: think about flexible array members? */
/* TODO: Not required: Implement ./minishell -c '' functionality */
/* TODO: check that where next is called, in case of early return,
 * that llast is called */
/* TODO: rigorously test list functions */
/* TODO: (void) cast all functions where return value is not used */
/* TODO: Remove asserts */
/* TODO: builtin simple_command vs simple_command in a pipeline */
/* TODO: test executing directories */
/* TODO: add default PATH */
/* TODO: heredoc history */
/* TODO: set default ifs, do not inherit */
/* TODO: set default hostname to localhost in ft_gethostname */
/* TODO: Empty IFS? */
/* TODO: Empty delimiter with ljoin? */
/* TODO: Empty delimiter with lsplit? */
/* TODO: Usage infos for builtins */
/* TODO: help builtin */
/* TODO: shift builtin */
/* TODO: get SHLVL right */
/* TODO: get sourcing right */
/* TODO: improve sourcing (source in the middle of a line?
 * sourcing when -n is enabled?) */
/* TODO: Fix -t with sourcing */
/* TODO: Seperate lines error msgs for heredoc */
/* close(3); close(63); */ /* valgrind */
int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	init(argv, envp);
	repl();
	finish(true);
	return (get_last_exit_status());
}
