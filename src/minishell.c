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
		finish();
		exit(exit_code);
	}
	return (exit_code);
}

char	*expand_prompt(char *prompt_string)
{
	t_list	*replacements;

	replacements = lnew();
	lpush(replacements, as_str_pair(&(t_str_pair){"\\u", env_lookup("USER")}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\w", ft_getcwd()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\W", lsplit(ft_getcwd(), "/")->last->as_str}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\h", ft_gethostname()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\H", ft_split(ft_gethostname(), '.')[0]}));
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
/* TODO: rigorously test list functions */
/* TODO: (void) cast all functions where return value is not used */
int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_list		*tokens;
	t_tree		*ast_root_node;

	set_allocator(gc_malloc);
	inherit_environment(envp);
	((void)argc, set_argv(argv), set_env(envp));
	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();
	set_var("?", "0", (t_flags){.special = true});
	set_var("OLDPWD", gc_add_str(getcwd(NULL, 0)), (t_flags){0});
	set_var("PWD", gc_add_str(getcwd(NULL, 0)), (t_flags){0});
	set_var("A", ft_strdup("file1.txt * foo.txt"), (t_flags){.exp = true});
	while (1)
	{
		set_var("PS0", expand_prompt(PS0), (t_flags){0});
		set_var("PS1", expand_prompt(PS1), (t_flags){0});
		line = gc_add_str(readline(get_var("PS1")->value));
		/* line = ft_strdup("echo hi > $A"); */
		if (!line)
			break ;
		add_history(line);
		tokens = tokenize(line);
		lprint(tokens, print_token);
		ast_root_node = build_ast(tokens);
		tree_print(ast_root_node);
		set_var("?", ft_itoa(execute(ast_root_node)), (t_flags){.special = true});
		(void)gc_free("DEFAULT");
	}
	finish();
	return (0);
}
/*














































*/
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
	(void)gc_free_all();
}

/* TODO: make logic correct */
void	set_pwd(void)
{
	char	*cwd;

	cwd = get_var("PWD")->value;
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

	cwd = get_var("PWD")->value;
	if (NULL != cwd)
	{
		set_var("OLDPWD", cwd, (t_flags){0});
	}
}

void	set_initial_shell_variables(void)
{
	/* inherit_environment(); */
	set_var("?", "0", (t_flags){.special = true});
	set_pwd();
}

void	init(void)
{
	set_allocator(gc_malloc);
	gc_set_context("DEFAULT");
	set_initial_shell_variables();
}

int	main2(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;
	init();
	repl();
	finish();
	return (0);
}
