#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h>             /* needed for FILE typedef needed by readline */
#include <readline/history.h>  /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h>            /* exit(), DONT USE free or malloc! */
#include <unistd.h>            /* STDERR_FILENO */

/* TODO: use argv[0] instead of hardcoded minishell */
void	minishell_error(int exit_code, const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	ft_vdprintf(STDERR_FILENO, ft_strjoin(ft_strjoin("minishell: ", fmt), "\n"),
		ap);
	va_end(ap);
	free_all_ptrs();
	exit(exit_code);
}

/* TODO: NOT REQUIRED: add basic prompt expansion */
char	*expand_prompt(const char *prompt_string)
{
	return (ft_strdup(prompt_string));
}

void	update_state(t_state *state)
{
	state->ps0 = expand_prompt(PS0);
	state->ps1 = expand_prompt(PS1);
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
/* TODO: check all free_all_ptrs() calls */
/* TODO: make free_all_ptrs() callable multiple times */
/* TODO: find unused functions (like printing functions) */
int	main(int argc, char **argv, char **envp)
{
	static t_state	state;
	static int		i;
	char			*line;
	t_deque			*tokens;
	t_ast_node		*ast_root_node;

	i = 3;
	(void)argc;
	(void)argv;
	set_environ(envp);
	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();

	while (i--)
	{
		update_state(&state);
		/* line = manage_ptrs(readline(state.ps1))->head->prev->as_str; */
		line = "echo hi";
		if (!line)
			break ;
		tokens = tokenize(line);
		deque_print(tokens, print_token);
		ast_root_node = build_ast(tokens);
		ast_print(ast_root_node);
		execute(ast_root_node);
	}
	(void)free_all_ptrs();
	return (0);
}
