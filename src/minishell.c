#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h>             /* needed for FILE typedef needed by readline */
#include <readline/history.h>  /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h>            /* exit(), DONT USE free or malloc! */
#include <unistd.h>            /* STDERR_FILENO */

void	minishell_error(int exit_code, t_bool do_exit, const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	ft_vdprintf(STDERR_FILENO, ft_strjoin(ft_strjoin(MINISHELL_NAME ": ", fmt),
			"\n"), ap);
	va_end(ap);
	free_all_ptrs();
	if (do_exit)
		exit(exit_code);
}

size_t	ft_count_all(
	const char str[static 1],
	const char pattern[static 1])
{
	size_t	pattern_len;
	size_t	pattern_count;

	pattern_len = ft_strlen(pattern);
	pattern_count = 0;
	while (*str)
	{
		if (!ft_strncmp(str, pattern, pattern_len))
		{
			pattern_count += 1;
			str += pattern_len;
			if (!pattern_len)
				++str;
		}
		else
			++str;
	}
	if (!pattern_len)
		pattern_count += 1;
	return (pattern_count);
}

char	*ft_replace_all(
	const char str[static 1],
	const char pattern[static 1],
	const char replacement[static 1])
{
	size_t	pattern_len;
	size_t	replacement_len;
	int		str_idx;
	int		new_str_idx;
	char	*new_str;

	pattern_len = ft_strlen(pattern);
	replacement_len = ft_strlen(replacement);
	new_str = ft_malloc(sizeof(*new_str) * (1 + ft_strlen(str)
				+ (replacement_len - pattern_len) * ft_count_all(str, pattern)));
	str_idx = 0;
	new_str_idx = 0;
	while (str[str_idx])
	{
		if (!ft_strncmp(str + str_idx, pattern, pattern_len))
		{
			ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
			str_idx += pattern_len;
			new_str_idx += replacement_len;
			if (!pattern_len)
				new_str[new_str_idx++] = str[str_idx++];
		}
		else
			new_str[new_str_idx++] = str[str_idx++];
	}
	if (!pattern_len)
	{
		ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
		new_str_idx += replacement_len;
	}
	new_str[new_str_idx] = 0;
	return (new_str);
}

#define UNIQUE1 "\xbe \xd8 \x85 \x52 \xa3"
#define UNIQUE2 "\xa1 \x3f \x6f \x28 \x87"
char	*uniquize(const char *str)
{
	return (ft_strjoin(UNIQUE1, ft_strjoin(str, UNIQUE2)));
}

/* TODO: NOT REQUIRED: add basic prompt expansion */
char	*expand_prompt(const char *prompt_string)
{
	char	*str;
	str = ft_strdup(prompt_string);
	str = ft_replace_all(str, "\\u", uniquize("\\u"));
	/* str = ft_replace_all(str, "\\h", uniquize("\\h")); */
	str = ft_replace_all(str, "\\w", uniquize("\\w"));
	/* str = ft_replace_all(str, "\\W", uniquize("\\W")); */
	str = ft_replace_all(str, uniquize("\\u"), env_lookup("USER"));
	str = ft_replace_all(str, uniquize("\\w"), manage_ptr(getcwd(NULL, 0))->head->prev->as_str);
	return (str);
}

void	update_state(t_state *state)
{
	state->ps0 = expand_prompt(PS0);
	state->ps1 = expand_prompt(PS1);
}

/* TODO: history */
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
/* TODO: write test functions for deque */
/* TODO: use -MMD and stuff */
/* TODO: revise all minishell_error() calls */
/* TODO: remove unnecessary indentation in variables declarations */
/* TODO: use ptr[static 1] where needed */
/* TOOD: flexible array members? */
int	main(int argc, char **argv, char **envp)
{
	static t_state	state;
	static int		i;
	char			*line;
	t_deque			*tokens;
	t_ast_node		*ast_root_node;

	i = 10;
	((void)argc, set_argv(argv), set_env(envp));
	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();
	while (i--)
	{
		update_state(&state);
		line = manage_ptr(readline(state.ps1))->head->prev->as_str;
		if (!line)
			break ;
		tokens = tokenize(line);
		/* deque_print(tokens, print_token); */
		ast_root_node = build_ast(tokens);
		/* ast_print(ast_root_node); */
		(void)execute(ast_root_node);
		(void)free_all_ptrs();
	}
	(void)free_all_ptrs();
	return (0);
}
