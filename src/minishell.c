#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h>             /* needed for FILE typedef needed by readline */
#include <readline/history.h>  /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h>            /* exit(), DONT USE free or malloc! */
#include <string.h>
#include <unistd.h>            /* STDERR_FILENO */

void	minishell_error(int exit_code, t_bool do_exit, const char *fmt, ...)
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

char	*ft_replace(
	const char str[static 1],
	const char pattern[static 1],
	const char replacement[static 1])
{
	size_t	pattern_len;
	size_t	replacement_len;
	size_t	str_idx;
	size_t	new_str_idx;
	t_bool	replaced;
	char	*new_str;

	pattern_len = ft_strlen(pattern);
	replacement_len = ft_strlen(replacement);
	if (ft_count_all(str, pattern) == 0)
		return (ft_strdup(str));
	new_str = ft_malloc(sizeof(*new_str) * (1 + ft_strlen(str)
				+ (replacement_len - pattern_len)));
	str_idx = 0;
	new_str_idx = 0;
	replaced = FALSE;
	while (str[str_idx])
	{
		if (!ft_strncmp(str + str_idx, pattern, pattern_len) && !replaced)
		{
			replaced = TRUE;
			ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
			str_idx += pattern_len;
			new_str_idx += replacement_len;
			if (!pattern_len)
				new_str[new_str_idx++] = str[str_idx++];
		}
		else
			new_str[new_str_idx++] = str[str_idx++];
	}
	if (!pattern_len && !replaced)
	{
		ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
		new_str_idx += replacement_len;
	}
	new_str[new_str_idx] = 0;
	return (new_str);
}

char	*ft_replace_all(
	const char str[static 1],
	const char pattern[static 1],
	const char replacement[static 1])
{
	size_t	pattern_len;
	size_t	replacement_len;
	size_t	str_idx;
	size_t	new_str_idx;
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

/* #define UNIQUE1 "\xbe \xd8 \x85 \x52 \xa3" */
/* #define UNIQUE2 "\xa1 \x3f \x6f \x28 \x87" */
#define UNIQUE1 "<START_UNIQUE>"
#define UNIQUE2 "<END_UNIQUE>"
char	*uniquize(const char *str)
{
	return (ft_strjoin(UNIQUE1, ft_strjoin(str, UNIQUE2)));
}

#include <fcntl.h>

char	*ft_gethostname(void)
{
	int		fd;
	char	*line;
	size_t	len;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return ("");
	line = get_next_line(fd);
	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		line[len - 1] = 0;
	close(fd);
	get_next_line(fd);
	if (line == NULL)
		return ("");
	return (line);
}

char	*ft_getcwd(void)
{
	char	*cwd;
	char	*home_dir;

	cwd = gc_add(getcwd(NULL, 0))->head->prev->as_str;
	home_dir = env_lookup("HOME");
	if (!ft_strncmp(home_dir, cwd, ft_strlen(home_dir)))
		cwd = ft_replace(cwd, home_dir, "~");
	return (cwd);
}

typedef struct s_str_pair
{
	char	*l;
	char	*r;
}			t_str_pair;

char	*expand_prompt(char *prompt_string)
{
	t_deque	*replacements;
	t_di	*di;

	replacements = deque_init();
	deque_push_ptr_right(replacements, &(t_str_pair){"\\u", env_lookup("USER")});
	deque_push_ptr_right(replacements, &(t_str_pair){"\\w", ft_getcwd()});
	deque_push_ptr_right(replacements, &(t_str_pair){"\\W", string_list_to_deque(ft_split(ft_getcwd(), '/'), (void *(*)(char *))ft_strdup)->head->prev->as_str});
	deque_push_ptr_right(replacements, &(t_str_pair){"\\h", ft_gethostname()});
	deque_push_ptr_right(replacements, &(t_str_pair){"\\H", ft_split(ft_gethostname(), '.')[0]});
	di = di_begin(replacements);
	while (di_next(di))
		prompt_string = ft_replace_all(prompt_string, (*(t_str_pair *)di_get(di)->as_ptr).l, uniquize((*(t_str_pair *)di_get(di)->as_ptr).l));
	di = di_begin(replacements);
	while (di_next(di))
		prompt_string = ft_replace_all(prompt_string, uniquize((*(t_str_pair *)di_get(di)->as_ptr).l), (*(t_str_pair *)di_get(di)->as_ptr).r);
	return (prompt_string);
}

void	update_state(t_state *state)
{
	state->ps0 = expand_prompt(PS0);
	state->ps1 = expand_prompt(PS1);
}

# define MAX_HT_SIZE 1000
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
int	main(int argc, char **argv, char **envp)
{
	static t_state	state;
	char			*line;
	t_deque			*tokens;
	t_ast_node		*ast_root_node;
	static t_kv		*shell_vars[MAX_HT_SIZE];

	((void)argc, set_argv(argv), set_env(envp));
	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();
	while (1)
	{
		update_state(&state);
		line = gc_add(readline(state.ps1))->head->prev->as_str;
		if (!line)
			break ;
		add_history(line);
		tokens = tokenize(line);
		/* deque_print(tokens, print_token); */
		ast_root_node = build_ast(tokens);
		/* ast_print(ast_root_node); */
		ht_set_malloc(shell_vars, "?", (t_type){ .as_str = ft_itoa_malloc(execute(ast_root_node))});
		(void)gc_free();
		ft_printf("? = %s\n", ht_get_malloc(shell_vars, "?").as_str);
	}
	ht_destroy(shell_vars);
	rl_clear_history();
	(void)gc_free();
	return (0);
}
