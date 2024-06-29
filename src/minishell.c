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
		set_var("CURRENT_LINE", lines->first->as_str, (t_flags){0});
		tree = parse(lines->first->as_str);
		if (heredoc_aborted(-1) == false || tree == NULL)
			exec(tree);
		set_var("LINENO", ft_itoa(ft_atoi(var_lookup("LINENO")) + 1), get_flags("LINENO"));
		lpop_left(lines);
		if (get_var("MINISHELL_SOURCE_EXECUTION_STRING") && get_var("MINISHELL_SOURCE_EXECUTION_STRING")->value)
		{
			lextend(lines, lsplit(var_lookup("MINISHELL_SOURCE_EXECUTION_STRING"), "\n"));
			set_var("MINISHELL_SOURCE_EXECUTION_STRING", NULL, get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
		}
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

	if (shopt_enabled('c'))
		return (lsplit(var_lookup("MINISHELL_EXECUTION_STRING"), "\n"));
	ps1 = expand_prompt(get_var("PS1")->value); // TOOD: Can we ensure that there's always PS1?
	if (isatty(STDIN_FILENO))
	{
		interactive_signals();
		rl_outstream = stderr;
		input = readline(ps1);
		noninteractive_signals();
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
		return (lnew());
	if (*input && ft_strcmp(input, prev_input))
		add_history(input);
	if (shopt_enabled('n'))
		ft_dprintf(STDERR_FILENO, "%s\n", input);
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
		if (lines->len == 0)
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

static void	set_shell_options(char *const argv[])
{
	t_list	*options;
	bool	has_c;
	char	erropt;
	char	*opts;
	int		i;
	bool	implicit_s;

	implicit_s = false;
	has_c = false;
	options = liter(ft_getopt_plus(argv, "acefilnstuvxC", &erropt, &optind));
	if (erropt)
	{
		(void)minishell_error(2, false, false, "-%c: invalid option", erropt);
		ft_dprintf(STDERR_FILENO, "Usage:\t%s [option] ...\n\t%s [option] script-file ...\nShell options:\n\t-ils or -c command (invocation only)\n\t-aefnstuvxC\n", MINISHELL_NAME, MINISHELL_NAME);
		finish(false);
		exit(2);
	}
	opts = "";
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

void	init(char *argv[], char *envp[])
{
	set_allocator(gc_malloc);
	gc_set_context("DEFAULT");
	set_initial_shell_variables(argv, envp);
	set_shell_options(argv);
	rl_event_hook = noop;
	/* rl_getc_function = msh_getc; */
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
/* TODO: ambiguous redirect */
/* TODO: add default PATH */
/* TODO: heredoc history */
/* TODO: unlink heredoc */
/* TODO: set default ifs, do not inherit */
/* TODO: set default hostname to localhost in ft_gethostname */
/* TODO: Empty IFS? */
/* TODO: Empty delimiter with ljoin? */
/* TODO: Empty delimiter with lsplit? */
/* TODO: Usage infos for builtins */
/* TODO: help builtin */
/* TODO: Source builtin */
/* TODO: shift builtin */
/* TODO: ; control operator */
/* TODO: ASSIGNMENT_WORDS */
/* TODO: PS0, PS4 */
int	main(int argc, char *argv[], char *envp[])
{
	/* close(3); close(63); */
	(void)argc;
	init(argv, envp);
	repl();
	finish(true);
	return (get_last_exit_status());
}
