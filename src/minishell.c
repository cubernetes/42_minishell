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

int	minishell_error(int exit_code, bool do_exit, const char *fmt, ...)
{
	va_list	ap;

	if (isatty(STDIN_FILENO) && do_exit)
		ft_dprintf(STDERR_FILENO, "exit\n");
	va_start(ap, fmt);
	ft_vdprintf(STDERR_FILENO, ft_strjoin(ft_strjoin(MINISHELL_NAME ": ", fmt), "\n"), ap);
	va_end(ap);
	if (do_exit)
	{
		finish(false);
		exit(exit_code);
	}
	return (exit_code);
}

static char	*get_dollar_prompt(void)
{
	if (ft_atoi(ft_getuid()) == 0)
		return ("#");
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

	liter(lines);
	while (lnext(lines))
	{
		tree = parse(lines->current->as_str);
		if (heredoc_aborted(-1) == false || tree == NULL)
			exec(tree);
	}
}

t_list	*get_lines(int fd)
{
	char		*input;
	char		*ps1;
	static char	*prev_input = "";

	if (isatty(fd))
	{
		ps1 = get_var("PS1")->value;
		interactive_signals();
		input = readline(ps1);
		noninteractive_signals();
		gc_add(input);
	}
	else
	{
		noninteractive_signals();
		input = get_next_line(fd);
	}
	if (input == NULL)
		return (lnew());
	if (*input && ft_strcmp(input, prev_input))
		add_history(input);
	gc_start_context("POST");
	prev_input = ft_strdup(input);
	gc_end_context();
	return (lsplit(input, "\n"));
}

void	expand_prompts(void)
{
	set_var("PS0", expand_prompt(PS0), get_flags("PS0"));
	set_var("PS1", expand_prompt(PS1), get_flags("PS1"));
	set_var("PS2", expand_prompt(PS2), get_flags("PS2"));
}

/* read-eval-print-loop */
void	repl(void)
{
	t_list	*lines;

	while (true)
	{
		expand_prompts();
		lines = get_lines(STDIN_FILENO);
		if (lines->len == 0)
			break ;
		interpret_lines(lines);
		gc_free("DEFAULT");
	}
}

void	finish(bool print_exit)
{
	if (print_exit && isatty(STDIN_FILENO))
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
	set_pwd();
}

static int	noop(void)
{
	return (0);
}

void	init(char *argv[], char *envp[])
{
	set_allocator(gc_malloc);
	gc_set_context("DEFAULT");
	set_initial_shell_variables(argv, envp);
	rl_event_hook = noop;
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
/* TODO: set ifs */
int	main(int argc, char *argv[], char *envp[])
{
	close(3); close(63);
	(void)argc;
	init(argv, envp);
	repl();
	finish(true);
	return (0);
}
