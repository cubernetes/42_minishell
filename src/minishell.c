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

	va_start(ap, fmt);
	ft_vdprintf(STDERR_FILENO, ft_strjoin(ft_strjoin(MINISHELL_NAME ": ", fmt),
			"\n"), ap);
	va_end(ap);
	if (do_exit)
	{
		gc_free();
		rl_clear_history();
		exit(exit_code);
	}
	return (exit_code);
}

typedef struct s_str_pair
{
	char	*l;
	char	*r;
}			t_str_pair;

char	*expand_prompt(char *prompt_string)
{
	t_list	*replacements;

	replacements = lnew();
	lpush(replacements, as_ptr(&(t_str_pair){"\\u", env_lookup("USER")}));
	lpush(replacements, as_ptr(&(t_str_pair){"\\w", ft_getcwd()}));
	lpush(replacements, as_ptr(&(t_str_pair){"\\W", string_list_to_deque(ft_split(ft_getcwd(), '/'), (void *(*)(char *))ft_strdup)->head->prev->as_str}));
	lpush(replacements, as_ptr(&(t_str_pair){"\\h", ft_gethostname()}));
	lpush(replacements, as_ptr(&(t_str_pair){"\\H", ft_split(ft_gethostname(), '.')[0]}));
	liter(replacements);
	while (lnext(replacements))
		prompt_string = ft_replace_all(prompt_string, (*(t_str_pair *)replacements->current->as_ptr).l, uniquize((*(t_str_pair *)replacements->current->as_ptr).l));
	liter(replacements);
	while (lnext(replacements))
		prompt_string = ft_replace_all(prompt_string, uniquize((*(t_str_pair *)replacements->current->as_ptr).l), (*(t_str_pair *)replacements->current->as_ptr).r);
	return (prompt_string);
}

/* TODO: what if readline returns NULL? */
/* TODO: use/think about rl_end (and other rl vars) */
/* TODO: remove DEBUG macros */
/* TODO: remove deque_print(tokens, print_token); */
/* TODO: glob expansion, env var expansion, and quote handling */
/* TOOD: parsing */
/* TODO: execution */
/* TODO: signal handling */
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
/* TODO: Not required: implement shell variables */
/* TODO: Implement ./minishell -c '' functionality */
/* TODO: Use yoda conditions */
/* TODO: check that where next is called, in case of early return, that llast is called */
int	main2(int argc, char **argv, char **envp)
{
	char		*line;
	t_list		*tokens;
	t_tree		*ast_root_node;

	set_allocator(gc_malloc);
	((void)argc, set_argv(argv), set_env(envp));
	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();
	set_var("?", ft_strdup("0"), (t_flags){0});
	set_var("OLDPWD", gc_add_str(getcwd(NULL, 0)), (t_flags){0});
	set_var("PWD", gc_add_str(getcwd(NULL, 0)), (t_flags){0});
	while (1)
	{
		set_var("PS0", expand_prompt(PS0), (t_flags){0});
		set_var("PS1", expand_prompt(PS1), (t_flags){0});
		line = gc_add_str(readline(get_var("PS1")));
		if (!line)
			break ;
		add_history(line);
		tokens = tokenize(line);
		/* deque_print(tokens, print_token); */
		ast_root_node = build_ast(tokens);
		/* ast_print(ast_root_node); */
		set_var("?", ft_itoa(execute(ast_root_node)), (t_flags){0});
		(void)gc_free();
	}
	/* clear_vars(); */
	rl_clear_history();
	(void)gc_free();
	return (0);
}



t_tree	*parse(char *line)
{
	t_list	*tokens;
	t_tree	*tree;

	tokens = tokenize(line);
	tree = build_ast(tokens);
	return (tree);
}

void	exec(t_tree *tree)
{
	print_tree_node(tree, true);
}

void	interpret_lines(t_list *lines)
{
	t_tree	*tree;

	liter(lines);
	while (lnext(lines))
	{
		tree = parse(lines->current->as_str);
		exec(tree);
	}
}

/* read-eval-print-loop */
void	repl(void)
{
	char	*input;
	t_list	*lines;

	while (true)
	{
		if (isatty(STDIN_FILENO))
		{
			input = readline(">>> ");
			gc_add_str(input);
		}
		else
			input = get_next_line(STDIN_FILENO);
		if (input == NULL)
			break ;
		lines = lsplit(input, "\n");
		interpret_lines(lines);
	}
}

void	finish(void)
{
	rl_clear_history();
	gc_free();
}

/* TODO: make logic correct */
void	set_pwd(void)
{
	char	*cwd;

	cwd = get_var("PWD");
	if (NULL != cwd)
	{
		cwd = getcwd(NULL, 0);
		gc_add_str(cwd);
	}
	set_var("PWD", cwd, (t_flags){.exp = true});
}
/* On startup, bash sets the value of PWD to getcwd(2) when it is unset.
 * However, when it is set already (through inheritance), then it is not
 * updated until the next chdir(2) UNLESS the directory described by PWD
 * does not exist, does not refer to the same inode number as the directory
 * described by getcwd(2), in which case it is set to getcwd(2).
 */

void	set_oldpwd(void)
{
	char	*cwd;

	cwd = get_var("PWD");
	if (NULL != cwd)
	{
		set_var("OLDPWD", cwd, (t_flags){0});
	}
}

void	inherit_environment(void)
{
}

void	set_initial_shell_variables(void)
{
	inherit_environment();
	set_var("?", "0", (t_flags){.exp = true, .hidden = true});
	set_pwd();
}

void	init(void)
{
	set_allocator(gc_malloc);
	/* gc_set_context("DEFAULT"); */
	set_initial_shell_variables();
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;
	init();
	repl();
	finish();
}
