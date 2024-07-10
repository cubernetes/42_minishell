/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:04:12 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 15:55:10 by pgrussin         ###   ########.fr       */
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

/* read-eval-print-loop */
void	repl(void)
{
	t_list			*lines;
	t_list			*real_lines;
	unsigned char	exit_status;

	while (true)
	{
		lines = get_lines(STDIN_FILENO);
		real_lines = lnew();
		if (lines == NULL)
			break ;
		if (option_enabled('t'))
			lpush(real_lines, as_data(lines->first));
		else
			lextend(real_lines, lines);
		exit_status = interpret_lines(real_lines);
		if (option_enabled('t') || option_enabled('c')
			|| (exit_status != 0 && option_enabled('e')))
			break ;
		gc_free("DEFAULT");
	}
}

void	finish(bool print_exit)
{
	if (print_exit && !option_enabled('t') && option_enabled('i')
		&& !option_enabled('c') && !(get_last_exit_status() != 0
			&& option_enabled('e')))
		ft_dprintf(STDERR_FILENO, "exit\n");
	rl_clear_history();
	(void)gc_free_all();
}

/* wtf */
static void	setup_pwd(void)
{
	unsigned long	cwd_inode;
	unsigned long	real_cwd_inode;
	t_var			*cwd_path;
	char			*real_path;
	struct stat		cwd_stat;
	struct stat		real_cwd_stat;

	cwd_path = get_var("PWD");
	if (cwd_path != NULL)
	{
		if (stat(cwd_path->value, &cwd_stat) < 0)
		{
			/* minishell_error(1, false, false, "error getting stat of pwd: %s", cwd_path->value); */
			real_path = gc_add_str(getcwd(NULL, 0));
			if (real_path == NULL)
			{
				minishell_error(0, false, false, "!shell-init: error retrieving current directory: getcwd: cannot access parent directory: %s", strerror(errno));
				set_var("PWD", cwd_path->value, (t_flags){.exp = true}); // subsequent cd must set PWD to .. or smth
				set_saved_cwd(cwd_path->value); // subsequent pwd must fail and cd should PWD change it to . or .. or smth afterwards
			}
			else
			{
				set_var("PWD", real_path, (t_flags){.exp = true});
				set_saved_cwd(real_path);
			}
		}
		else
		{
			cwd_inode = cwd_stat.st_ino;
			real_path = gc_add_str(getcwd(NULL, 0));
			if (real_path == NULL)
			{
				minishell_error(0, false, false, "!shell-init: error retrieving current directory: getcwd: cannot access parent directory: %s", strerror(errno));
				set_var("PWD", cwd_path->value, (t_flags){.exp = true}); // subsequent cd must set PWD to .. or smth
				set_saved_cwd(""); // subsequent pwd must fail and cd should PWD change it to . or .. or smth afterwards
			}
			else
			{
				if (stat(real_path, &real_cwd_stat) < 0)
				{
					/* minishell_error(1, false, false, "error getting stat of real_path: %s", cwd_path->value); */
					set_var("PWD", real_path, (t_flags){.exp = true});
					set_saved_cwd(real_path);
				}
				else
				{
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
			}
		}
	}
	else
	{
		real_path = gc_add_str(getcwd(NULL, 0));
		if (real_path == NULL)
		{
			minishell_error(0, false, false, "!shell-init: error retrieving current directory: getcwd: cannot access parent directory: %s", strerror(errno));
			set_saved_cwd(""); // subsequent pwd must fail and cd should PWD change it to . or .. or smth afterwards
								// PWD stays empty, \w expands to ""
		}
		else
		{
			set_var("PWD", real_path, (t_flags){.exp = true}); // subsequent pwd must fail and cd should PWD change it to . or .. or smth afterwards
			set_saved_cwd(real_path); // PWD gets set, \w expands to PWD
		}
	}
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

static int	noop(void)
{
	return (0);
}

// default
// (none)
// default -i
// i
// default interactive (i cannot be removed)
// is (adds i and s)
// default non-interactive
// s (adds s)
// default -c
// c (adds c)
// default non-interactive with -i
// is (adds i and s)
// default -c with -i
// ic (adds i and c)
//
// + - doesn't matter (cannot be negated)
// cs
// + - does matter, last one counts
// aefintuvxC

static int	set_shell_options(char *const argv[])
{
	t_list	*options;
	char	erropt;
	char	*opts;
	int		i;
	bool	implicit_s;
	int		tty;
	int		is_login_shell;

	implicit_s = false;
	options = liter(ft_getopt_plus(argv, "acefilnstuvxC", &erropt, &optind));
	if (erropt)
	{
		(void)minishell_error(2, false, false, "-%c: invalid option", erropt);
		ft_dprintf(STDERR_FILENO, "Usage:\t%s [option] ...\n\t%s [option] script-file ...\nShell options:\n\t-ils or -c command (invocation only)\n\t-aefnstuvxC\n", MINISHELL_NAME, MINISHELL_NAME);
		finish(false);
		exit(2);
	}
	opts = "";
	is_login_shell = false;
	liter(options);
	while (lnext(options))
	{
		if ((char)options->current->as_getopt_arg == 'c')
		{
			if (!ft_strchr(opts, 'c'))
				opts = ft_strjoin(opts, "c");
		}
		else if ((char)options->current->as_getopt_arg == 's')
		{
			if (!ft_strchr(opts, 's'))
				opts = ft_strjoin(opts, "s");
		}
		else if (options->current->as_getopt_arg & 1 << 8 && ft_strchr(opts, (char)options->current->as_getopt_arg))
			opts = ljoin(lsplit(opts, ft_strndup(&(char){(char)options->current->as_getopt_arg}, 1)), "");
		else if (!ft_strchr(opts, (char)options->current->as_getopt_arg) && (char)options->current->as_getopt_arg != 'l')
			opts = ft_strjoin(opts, ft_strndup(&(char){(char)options->current->as_getopt_arg}, 1));
		else if ((char)options->current->as_getopt_arg == 'l')
			is_login_shell = true;
	}
	if (ft_strchr(opts, 'i'))
	{
		tty = open("/dev/tty", O_WRONLY);
		if (tty == -1)
			(void)minishell_error(1, false, false, "/dev/tty: ", strerror(errno));
		else
		{
			dup2(tty, STDERR_FILENO);
			close(tty);
		}
	}
	if (!ft_strchr(opts, 'c'))
	{
		if (!ft_strchr(opts, 's'))
		{
			opts = ft_strjoin(opts, "s");
			implicit_s = true;
		}
		if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
			if (!ft_strchr(opts, 'i'))
				opts = ft_strjoin(opts, "i");
	}
	set_var("-", opts, (t_flags){.special = true});
	set_var("0", argv[0], (t_flags){.special = true});
	is_login_shell |= argv[0][0] == '-';
	argv += optind;
	if (ft_strchr(opts, 'c'))
	{
		if (*argv == NULL)
			minishell_error(2, true, false, "-c: option requires an argument");
		set_var("MINISHELL_EXECUTION_STRING", *argv, (t_flags){0});
		++argv;
	}
	if (ft_strchr(opts, 'c') || (ft_strchr(opts, 's') && !implicit_s))
	{
		i = 1;
		if (ft_strchr(opts, 'c'))
			i = 0;
		while (*argv)
		{
			set_var(ft_itoa(i), *argv, (t_flags){.special = true});
			++argv;
			++i;
		}
		set_var("#", ft_itoa(i - 1), (t_flags){.special = true});
	}
	else if (*argv != NULL)
	{
		ft_printf("Not implemented yet...\n");
		// run/source script or smth
	}
	return (is_login_shell);
}

/* static int	msh_getc(FILE *stream) */
/* { */
	/* char	c; */
/*  */
	/* read(1, &c, 1); */
	/* if (isatty(STDERR_FILENO)) */
		/* return (c); */
	/* ft_dprintf(0, "%c", c); */
	/* if (c == '\x7f') */
		/* ft_dprintf(0 ,"\b \b"); */
	/* if (c == '\r') */
		/* ft_dprintf(0 ,"\n"); */
	/* return (c); */
/* } */

static void	read_init_files(bool is_login_shell)
{
	char	*profile;
	char	*rc;

	profile = ft_strjoin(var_lookup("HOME"), "/.msh_profile");
	rc = ft_strjoin(var_lookup("HOME"), "/.mshrc");
	if (is_login_shell)
	{
		if (!access(profile, R_OK) && open(profile, O_DIRECTORY) == -1)
			set_var("MINISHELL_SOURCE_EXECUTION_STRING", "source \"$HOME/.msh_profile\"", get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
	}
	else if (option_enabled('i'))
		if (!access(rc, R_OK) && open(rc, O_DIRECTORY) == -1)
			set_var("MINISHELL_SOURCE_EXECUTION_STRING", "source \"$HOME/.mshrc\"", get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
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
	/* rl_getc_function = msh_getc; */
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
/* TODO: check that where next is called, in case of early return, that llast is called */
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
/* TODO: improve sourcing (source in the middle of a line? sourcing when -n is enabled?) */
/* TODO: Fix -t with sourcing */
/* TODO: Seperate lines error msgs for heredoc */
int	main(int argc, char *argv[], char *envp[])
{
	/* close(3); close(63); */ /* valgrind */
	(void)argc;
	init(argv, envp);
	repl();
	finish(true);
	return (get_last_exit_status());
}
