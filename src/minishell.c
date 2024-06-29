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

int	minishell_error(int exit_code, bool do_exit, bool syntax_error, const char *fmt, ...)
{
	va_list	ap;
	char	*errfmt;
	char	*err;

	if (shopt_enabled('i') && do_exit)
		ft_dprintf(STDERR_FILENO, "exit\n");
	va_start(ap, fmt);
	errfmt = ft_strjoin(var_lookup("0"), ": ");
	/* errfmt = ft_strjoin("bash", ": "); */
	if (!shopt_enabled('i'))
	{
		if (shopt_enabled('c') && syntax_error)
			errfmt = ft_strjoin(errfmt, "-c: ");
		errfmt = ft_strjoin(errfmt, ft_strjoin("line ",
					ft_strjoin(var_lookup("LINENO"), ": ")));
	}
	errfmt = ft_replace_all(errfmt, "%", "%%");
	err = ft_strjoin(errfmt, fmt);
	err = ft_strjoin(err, "\n");
	ft_vdprintf(STDERR_FILENO, err, ap);
	va_end(ap);
	if (!shopt_enabled('i') && syntax_error)
	{
		err = ft_strjoin(errfmt, "`%s'\n");
		ft_dprintf(STDERR_FILENO, err, var_lookup("CURRENT_LINE"));
	}
	if (do_exit)
	{
		finish(false);
		exit(exit_code);
	}
	return (exit_code);
}

static char	*get_dollar_prompt(void)
{
	if (!ft_strcmp(ft_geteuid(), "0"))
		return ("#");
	else if (ft_geteuid()[0] == '\0')
		return ("?");
	return ("$");
}

/* TODO: Expand variables */
char	*expand_prompt(char *prompt_string)
{
	t_list	*replacements;

	replacements = lnew();
	lpush(replacements, as_str_pair(&(t_str_pair){"\\u", ft_getusername()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\w", ft_getcwd()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\W", lsplit(ft_getcwd(), "/")->last->as_str}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\h", ft_gethostname()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\H", ft_split(ft_gethostname(), '.')[0]}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\$", get_dollar_prompt()}));
	liter(replacements);
	while (lnext(replacements))
		prompt_string = ft_replace_all(prompt_string,
				replacements->current->as_str_pair->l,
				uniquize(replacements->current->as_str_pair->l));
	liter(replacements);
	while (lnext(replacements))
		prompt_string = ft_replace_all(prompt_string,
				uniquize(replacements->current->as_str_pair->l),
				replacements->current->as_str_pair->r);
	return (prompt_string);
}

void	inherit_environment(char *envp[])
{
	t_list	*pair;

	while (*envp)
	{
		pair = lsplit_n(*envp, "=", 1);
		set_var(pair->first->as_str,
			pair->last->as_str,
			(t_flags){.exp = true});
		++envp;
	}
}

t_tree	*parse(char *line)
{
	t_list	*tokens;
	t_tree	*tree;

	tokens = tokenize(line);
	tree = build_ast(tokens, true);
	return (tree);
}

void	exec(t_tree *tree)
{
	unsigned char	exit_status;

	if (tree == NULL)
		exit_status = 0;
	else
		exit_status = execute_complete_command(tree);
	set_var("?", ft_itoa(exit_status), (t_flags){.special = true});
}

void	interpret_lines(t_list *lines)
{
	t_tree	*tree;

	while (lines->len > 0)
	{
		if (get_var("MINISHELL_SOURCE_EXECUTION_STRING") && get_var("MINISHELL_SOURCE_EXECUTION_STRING")->value)
		{
			lextend_left(lines, lsplit(var_lookup("MINISHELL_SOURCE_EXECUTION_STRING"), "\n"));
			set_var("MINISHELL_SOURCE_EXECUTION_STRING", NULL, get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
		}
		set_var("CURRENT_LINE", lines->first->as_str, (t_flags){0});
		if (shopt_enabled('v'))
			ft_dprintf(STDERR_FILENO, "%s\n", lines->first->as_str);
		tree = parse(lines->first->as_str);
		if (heredoc_aborted(-1) == false || tree == NULL)
			exec(tree);
		set_var("LINENO", ft_itoa(ft_atoi(var_lookup("LINENO")) + 1), get_flags("LINENO"));
		lpop_left(lines);
	}
}

bool	shopt_enabled(char opt)
{
	return (ft_strchr(var_lookup("-"), opt) != NULL);
}

t_list	*get_lines(int fd)
{
	char		*input;
	char		*ps1;
	static char	*prev_input = "";
	int			tty;
	int			old;
	bool		restore;

	if (shopt_enabled('c'))
		return (lsplit(var_lookup("MINISHELL_EXECUTION_STRING"), "\n"));
	else if (get_var("MINISHELL_SOURCE_EXECUTION_STRING") && get_var("MINISHELL_SOURCE_EXECUTION_STRING")->value)
	{
		input = var_lookup("MINISHELL_SOURCE_EXECUTION_STRING");
		set_var("MINISHELL_SOURCE_EXECUTION_STRING", NULL, get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
		return (lsplit(input, "\n"));
	}
	ps1 = expand_prompt(get_var("PS1")->value); // TOOD: Can we ensure that there's always PS1?
	if (isatty(STDIN_FILENO))
	{
		restore = false;
		if (!isatty(STDERR_FILENO))
		{
			restore = true;
			tty = open("/dev/tty", O_WRONLY);
			if (tty == -1)
				(void)minishell_error(1, false, false, "/dev/tty: ", strerror(errno));
			else
			{
				old = dup(STDERR_FILENO);
				dup2(tty, STDERR_FILENO);
				close(tty);
			}
			if (!shopt_enabled('i'))
				ps1 = "";
		}
		interactive_signals();
		input = readline(ps1);
		noninteractive_signals();
		if (restore)
		{
			dup2(old, STDERR_FILENO);
			close(old);
		}
		gc_add(input);
	}
	else
	{
		noninteractive_signals();
		input = get_next_line(fd);
		if (shopt_enabled('i') && input != NULL)
			ft_dprintf(STDERR_FILENO, "%s", ft_strjoin(ps1, input));
		else if (shopt_enabled('i'))
			ft_dprintf(STDERR_FILENO, "%s", ps1);
	}
	if (input == NULL)
		return (NULL);
	if (*input && ft_strcmp(input, prev_input))
		add_history(input);
	ft_dprintf(STDERR_FILENO, "%s", var_lookup("PS0"));
	gc_start_context("POST");
	prev_input = ft_strdup(input);
	gc_end_context();
	return (lsplit(input, "\n"));
}

/* read-eval-print-loop */
void	repl(void)
{
	t_list	*lines;

	while (true)
	{
		lines = get_lines(STDIN_FILENO);
		if (lines == NULL)
			break ;
		interpret_lines(lines);
		if (shopt_enabled('c'))
			break ;
		gc_free("DEFAULT");
	}
}

void	finish(bool print_exit)
{
	if (print_exit && (shopt_enabled('i') && !shopt_enabled('c')))
		ft_dprintf(STDERR_FILENO, "exit\n");
	rl_clear_history();
	(void)gc_free_all();
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
	set_argv(argv);
	set_var("?", "0", (t_flags){.special = true});
	unset_var("MINISHELL_XTRACEFD"); // TODO: If it has a value and is unset or set to a new value, the fd corresponding to the old value shall be closed.
	set_var("$", ft_getpid(), (t_flags){.special = true});
	set_var("PPID", ft_getppid(), (t_flags){.readonly = true});
	set_var("LINENO", "1", (t_flags){0});
	// TODO: Depends on if interactive or not
	set_var("SHLVL", ft_itoa(ft_atoi(var_lookup("SHLVL")) + 1), (t_flags){.exp = true});
	set_var("_", argv[0], (t_flags){0});
	set_var("PS0", PS0, (t_flags){0});
	set_var("PS1", PS1, (t_flags){0});
	set_var("PS2", PS2, (t_flags){0});
	set_var("PS4", PS4, (t_flags){0});
	set_var("MINISHELL_SOURCE_EXECUTION_STRING", NULL, (t_flags){.readonly = true});
	set_pwd();
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
	else if (shopt_enabled('i'))
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
int	main(int argc, char *argv[], char *envp[])
{
	/* close(3); close(63); */ /* valgrind */
	(void)argc;
	init(argv, envp);
	repl();
	finish(true);
	return (get_last_exit_status());
}
