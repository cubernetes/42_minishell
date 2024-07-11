/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 15:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/07/11 21:03:58 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
