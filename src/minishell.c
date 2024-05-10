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

#define MAX_HT_SIZE 1000

enum e_type
{
	INT,
	SIZE_T,
	CHAR,
	STR,
	PTR,
	LIST_NODE,
};

typedef struct s_list_node	t_list_node;
typedef struct s_list		t_list;

typedef struct s_data
{
	enum e_type		type;
	union
	{
		int			as_int;
		size_t		as_size_t;
		char		as_char;
		char		*as_str;
		void		*as_ptr;
		t_list_node	*as_list_node;
	};
}	t_data;

/* anonymous struct must come first and be identical to t_data */
struct s_list_node
{
	struct
	{
		enum e_type		type;
		union
		{
			int			as_int;
			size_t		as_size_t;
			char		as_char;
			char		*as_str;
			void		*as_ptr;
			t_list_node	*as_list_node;
		};
	};
	t_list_node	*next;
	t_list_node	*prev;
};

typedef struct s_list
{
	t_list_node		*first;
	t_list_node		*last;
	t_list_node		*current;
	size_t			current_idx;
	t_list			*current_stack;     /* don't iterate over this one obv. */
	t_list			*current_idx_stack; /* don't iterate over this one obv. */
	size_t			len;
}	t_list;

t_list	*new_list(void)
{
	return (ft_memdup(&(t_list){
			.current_stack = ft_memdup(&(t_list){0}, sizeof(t_list)),
			.current_idx_stack = ft_memdup(&(t_list){0}, sizeof(t_list))
		}, sizeof(t_list))
	);
}

t_data	as_str(char *as_str)
{
	return ((t_data){.as_str = as_str, .type = STR});
}

t_data	as_ptr(void *as_ptr)
{
	return ((t_data){.as_ptr = as_ptr, .type = PTR});
}

t_data	as_int(int as_int)
{
	return ((t_data){.as_int = as_int, .type = INT});
}

t_data	as_size_t(size_t as_size_t)
{
	return ((t_data){.as_size_t = as_size_t, .type = SIZE_T});
}

t_data	as_char(char as_char)
{
	return ((t_data){.as_char = as_char, .type = CHAR});
}

t_list	*push(t_list list[static 1], t_data data)
{
	struct s_list_node	*node;

	node = ft_malloc(sizeof(*node));
	ft_memmove(node, &data, sizeof(data));
	if (list->len == 0)
	{
		list->first = node;
		list->current = node;
		list->last = node;
	}
	else
	{
		list->last->next = node;
		list->first->prev = node;
		if (list->first == list->current)
			list->current->prev = node;
	}
	node->next = list->first;
	node->prev = list->last;
	list->last = node;
	list->len++;
	return (list);
}

/* void	destroy_simple_list(t_list list[static 1]) */
/* { */
	/* struct s_list_node	*head; */
/*  */
	/* if (list->first == NULL) */
	/* { */
		/* free(list); */
		/* return ; */
	/* } */
	/* head = list->first->next; */
	/* while (head != list->first) */
	/* { */
		/* head = head->next; */
		/* if (head->prev->type == STR) */
			/* free(head->prev->as_str); */
		/* free(head->prev); */
	/* } */
	/* if (head->type == STR) */
		/* free(head->as_str); */
	/* free(head); */
	/* free(list); */
/* } */

/* void	destroy_list(t_list list[static 1]) */
/* { */
	/* destroy_simple_list(list->current_stack); */
	/* destroy_simple_list(list->current_idx_stack); */
	/* destroy_simple_list(list); */
/* } */

t_list	*start_iterator(t_list list[static 1])
{
	(void)push(list->current_stack, as_ptr(list->current));
	(void)push(list->current_idx_stack, as_size_t(list->current_idx));
	if (list->first)
		list->current = list->first->prev;
	list->current_idx = (size_t)-1;
	return (list);
}

t_list	*split(const char s[static 1], const char delim[static 2])
{
	t_list	*list;
	int		token_start;
	int		token_end;
	size_t	delim_len;

	delim_len = ft_strlen(delim);
	list = new_list();
	if (delim_len == 0)
		return (list);
	token_start = 0;
	token_end = 0;
	while (s[token_end])
	{
		if (!ft_strncmp(s + token_end, delim, delim_len))
		{
			(void)push(list, as_str(ft_strndup(s + token_start, (size_t)(token_end - token_start))));
			token_end += delim_len;
			token_start = token_end;
			--token_end;
		}
		++token_end;
	}
	(void)push(list, as_str(ft_strndup(s + token_start, (size_t)(token_end - token_start))));
	return (list);
}

t_list	*pop(t_list *list)
{
	list->last->prev->next = list->first;
	list->first->prev = list->last->prev;
	list->last = list->last->prev;
	list->len--;
	return (list);
}

t_list_node	*next(t_list list[static 1])
{
	if (list->current_idx == list->len - 1)
	{
		list->current = list->current_stack->last->as_ptr;
		list->current_idx = list->current_idx_stack->last->as_size_t;
		(void)pop(list->current_stack);
		(void)pop(list->current_idx_stack);
		return (NULL);
	}
	list->current = list->current->next;
	list->current_idx++;
	return (list->current);
}

t_list *last(t_list list[static 1])
{
	list->current = list->current_stack->last->as_ptr;
	list->current_idx = list->current_idx_stack->last->as_size_t;
	(void)pop(list->current_stack);
	(void)pop(list->current_idx_stack);
	return (list);
}

char	*get_next_line(int fd)
{
	ssize_t	bytes_read;
	char	c;
	t_list	*chars;
	char	*str;

	chars = new_list();
	bytes_read = read(fd, &c, 1);
	while (bytes_read)
	{
		if (c == '\n')
			break ;
		push(chars, as_char(c));
		bytes_read = read(fd, &c, 1);
	}
	if (bytes_read == 0 && chars->len == 0)
		return (NULL);
	str = ft_malloc(sizeof(*str) * (chars->len + 1));
	str[chars->len] = 0;
	start_iterator(chars);
	while (next(chars))
		str[chars->current_idx] = chars->current->as_char;
	return (str);
}

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
	bool	replaced;
	char	*new_str;

	pattern_len = ft_strlen(pattern);
	replacement_len = ft_strlen(replacement);
	if (ft_count_all(str, pattern) == 0)
		return (ft_strdup(str));
	new_str = ft_malloc(sizeof(*new_str) * (1 + ft_strlen(str)
				+ (replacement_len - pattern_len)));
	str_idx = 0;
	new_str_idx = 0;
	replaced = false;
	while (str[str_idx])
	{
		if (!ft_strncmp(str + str_idx, pattern, pattern_len) && !replaced)
		{
			replaced = true;
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

	cwd = gc_add_str(getcwd(NULL, 0));
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

char	*set_var(char *key, char *value, bool export)
{
	static t_kv	*shell_vars[MAX_HT_SIZE];
	char		*ret;

	if (key == NULL && value == NULL)
	{
		ht_destroy(shell_vars);
		return ("");
	}
	if (key == NULL)
	{
		ret = ((t_var *)ht_get(shell_vars, value).as_ptr)->value;
		if (ret == NULL)
			return ("");
		return (ret);
	}
	set_allocator(malloc);
	ht_set(shell_vars, key,
		(t_type){.as_ptr = ft_memdup(&(t_var){
			.export = export,
			.readonly = false,
			.value = ft_strdup(value)
		}, sizeof(t_var))});
	set_allocator(gc_malloc);
	return (value);
}

char	*get_var(char *key)
{
	return (set_var(NULL, key, false));
}

void	clear_vars(void)
{
	set_var(NULL, NULL, false);
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
int	main2(int argc, char **argv, char **envp)
{
	char			*line;
	t_deque			*tokens;
	t_ast_node		*ast_root_node;

	set_allocator(gc_malloc);
	((void)argc, set_argv(argv), set_env(envp));
	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();
	set_var("?", ft_strdup("0"), false);
	set_var("OLDPWD", gc_add_str(getcwd(NULL, 0)), false);
	set_var("PWD", gc_add_str(getcwd(NULL, 0)), false);
	while (1)
	{
		set_var("PS0", expand_prompt(PS0), false);
		set_var("PS1", expand_prompt(PS1), false);
		line = gc_add_str(readline(get_var("PS1")));
		if (!line)
			break ;
		add_history(line);
		tokens = tokenize(line);
		/* deque_print(tokens, print_token); */
		ast_root_node = build_ast(tokens);
		/* ast_print(ast_root_node); */
		set_var("?", ft_itoa(execute(ast_root_node)), false);
		(void)gc_free();
	}
	clear_vars();
	rl_clear_history();
	(void)gc_free();
	return (0);
}

char	*parse(char *line)
{
	return (line);
}

void	exec(char *cmd)
{
	printf("cmd:<%s>\n", cmd);
}

void	interpret_lines(t_list *lines)
{
	start_iterator(lines);
	while (next(lines))
		exec(parse(lines->current->as_str));
}

/* read-eval-print-loop */
void	repl(void)
{
	char	*input;

	while (true)
	{
		if (isatty(STDIN_FILENO))
			input = gc_add_str(readline(">>> "));
		else
			input = get_next_line(STDIN_FILENO);
		if (input == NULL)
			break ;
		interpret_lines(split(input, "\n"));
	}
}

void	finish(void)
{
	rl_clear_history();
	gc_free();
}

void	init(void)
{
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;
	set_allocator(gc_malloc);
	init();
	repl();
	finish();
}
