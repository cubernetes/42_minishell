#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <stdlib.h>
#include <assert.h>

const char	*ast_node_type_to_string(t_ast_node_type type)
{
	if (type == COMPLETE_COMMAND)
		return (STR_COMPLETE_COMMAND);
	else if (type == COMPLETE_COMMAND_TAIL)
		return (STR_COMPLETE_COMMAND_TAIL);
	else if (type == AND_OR)
		return (STR_AND_OR);
	else if (type == PIPE_SEQUENCE)
		return (STR_PIPE_SEQUENCE);
	else if (type == PIPE_SEQUENCE_TAIL)
		return (STR_PIPE_SEQUENCE_TAIL);
	else if (type == COMMAND)
		return (STR_COMMAND);
	else if (type == COMPOUND_COMMAND)
		return (STR_COMPOUND_COMMAND);
	else if (type == SIMPLE_COMMAND)
		return (STR_SIMPLE_COMMAND);
	else if (type == SIMPLE_COMMAND_TAIL)
		return (STR_SIMPLE_COMMAND_TAIL);
	else if (type == IO_REDIRECT)
		return (STR_IO_REDIRECT);
	else if (type == TOKEN)
		return (STR_TOKEN);
	else
		return (STR_AST_NODE_TYPE_UNKNOWN);
}

t_bool	ast_node_is_null(t_ast_node *ast_node)
{
	if (!ast_node)
		return (TRUE);
	if (ast_node->type == 0)
		if (ast_node->token == NULL && ast_node->children == NULL)
			return (TRUE);
	return (FALSE);
}

void	repeat_string(const char *str, int n, t_bool color)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		if (color)
		{
			ft_putstr("\033[");
			ft_putnbr((i % 7) + 31);
			ft_putstr("m");
		}
		ft_printf("%s", str);
		if (color)
			ft_putstr("\033[m");
	}
}

t_ast_node	*new_ast_token(t_token_type type, char *str)
{
	t_ast_node	*ast_node;

	ast_node = ft_malloc(sizeof(*ast_node));
	ast_node->type = TOKEN;
	ast_node->token = new_token(str, type, TRUE);
	return (ast_node);
}

t_ast_node	*new_ast_m_token(t_token_type type, const char *str)
{
	return (new_ast_token(type, ft_strdup(str)));
}

/* children must be allocated on heap */
t_ast_node	*new_ast_nonterm(t_ast_node_type type, t_deque *children)
{
	t_ast_node	*ast_node;

	ast_node = ft_malloc(sizeof(*ast_node));
	ast_node->type = type;
	ast_node->children = children;
	return (ast_node);
}

void	ast_print_with_depth(t_ast_node *ast_node, int n)
{
	t_deque_node	*head;

	repeat_string("|   ", n, TRUE);
	if (ast_node->type != TOKEN)
	{
		ft_printf("- <%s> (%d children)\n",
			ast_node_type_to_string(ast_node->type),
			ast_node->children->size);
		head = ast_node->children->head;
		if (!head)
			return ;
		ast_print_with_depth(head->as_ast_node, n + 1);
		while (head->next != ast_node->children->head)
		{
			head = head->next;
			ast_print_with_depth(head->as_ast_node, n + 1);
		}
	}
	else
	{
		ft_printf("- %s (\033[31m%s\033[m)\n",
			token_type_to_string(ast_node->token->type),
			ast_node->token->str);
	}
}

void	ast_print(t_ast_node *ast_node)
{
	ast_print_with_depth(ast_node, 0);
}

t_deque	*new_children(t_ast_node **children)
{
	t_deque	*new_children;
	size_t	i;

	i = 0;
	new_children = deque_init();
	while (children[i])
		deque_push_ptr_right(new_children, children[i++]);
	return (new_children);
}

t_ast_node	*return_example_parse_tree(void)
{
	return (\
	new_ast_nonterm(COMPLETE_COMMAND, new_children((t_children){\
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
			new_ast_nonterm(COMMAND, new_children((t_children){\
				new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
					new_ast_m_token(TOK_WORD, "echo"), \
					new_ast_m_token(TOK_WORD, "hi"), \
				NULL})), \
			NULL})), \
			new_ast_m_token(TOK_PIPE, "|"), \
			new_ast_nonterm(COMMAND, new_children((t_children){\
				new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
					new_ast_m_token(TOK_WORD, "grep"), \
					new_ast_m_token(TOK_WORD, "^h"), \
				NULL})), \
			NULL})),
		NULL})),
		new_ast_m_token(TOK_AND, "&&"),
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){
			new_ast_nonterm(COMMAND, new_children((t_children){
				new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){
					new_ast_m_token(TOK_WORD, "echo"),
					new_ast_m_token(TOK_SQUOTE_STR, "hello world"),
				NULL})),
			NULL})),
			new_ast_m_token(TOK_PIPE, "|"),
			new_ast_nonterm(COMMAND, new_children((t_children){
				new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){
					new_ast_m_token(TOK_WORD, "grep"),
					new_ast_m_token(TOK_DQUOTE_STR, "^hello world"),
				NULL})),
			NULL})),
		NULL})),
	NULL}))
	);
}

t_ast_node	*return_example_ast(void)
{
	return (\
	new_ast_nonterm(COMPLETE_COMMAND, new_children((t_children){\
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_HEREDOC, "<<"), \
					new_ast_m_token(TOK_WORD, "heredoc"), \
				NULL})), \
				new_ast_m_token(TOK_WORD, "echo"), \
				new_ast_m_token(TOK_WORD, "hi"), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_INPUT, "<"), \
					new_ast_m_token(TOK_WORD, "infile"), \
				NULL})), \
				new_ast_m_token(TOK_WORD, "grep"), \
				new_ast_m_token(TOK_WORD, "^h"), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_OVERRIDE, ">"), \
					new_ast_m_token(TOK_WORD, "somefile"), \
				NULL})), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_OVERRIDE, ">"), \
					new_ast_m_token(TOK_WORD, "anotherfile"), \
				NULL})), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_m_token(TOK_WORD, "nl"), \
			NULL})), \
		NULL})), \
		new_ast_m_token(TOK_AND, "&&"), \
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
			new_ast_nonterm(COMPLETE_COMMAND, new_children((t_children){\
				new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
					new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_HEREDOC, "<<"), \
							new_ast_m_token(TOK_WORD, "heredoc"), \
						NULL})), \
						new_ast_m_token(TOK_WORD, "echo"), \
						new_ast_m_token(TOK_WORD, "hi"), \
					NULL})), \
					new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_INPUT, "<"), \
							new_ast_m_token(TOK_WORD, "infile"), \
						NULL})), \
						new_ast_m_token(TOK_WORD, "grep"), \
						new_ast_m_token(TOK_WORD, "^h"), \
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_OVERRIDE, ">"), \
							new_ast_m_token(TOK_WORD, "somefile"), \
						NULL})), \
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_OVERRIDE, ">"), \
							new_ast_m_token(TOK_WORD, "anotherfile"), \
						NULL})), \
					NULL})), \
					new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
						new_ast_m_token(TOK_WORD, "nl"), \
					NULL})), \
				NULL})), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_m_token(TOK_WORD, "grep"), \
				new_ast_m_token(TOK_WORD, "h"), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_APPEND, ">>"), \
					new_ast_m_token(TOK_WORD, "appendfile.txt"), \
				NULL})), \
			NULL})), \
		NULL})), \
		new_ast_m_token(TOK_AND, "&&"), \
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_INPUT, "<"), \
					new_ast_m_token(TOK_WORD, "infile"), \
				NULL})), \
				new_ast_m_token(TOK_WORD, "hello world"), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_OVERRIDE, ">"), \
					new_ast_m_token(TOK_WORD, "/dev/null"), \
				NULL})), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_m_token(TOK_WORD, "grep"), \
				new_ast_m_token(TOK_DQUOTE_STR, "^hello world"), \
			NULL})), \
		NULL})), \
	NULL}))
	);
}

/* return the index of the production to use */
/* TODO: fix error handling */
int	get_production_idx(t_ast_node_type nonterm, t_token *token)
{
	static int	transition_table[NUM_NONTERMS][NUM_TOKENS] = {
	{19, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0},
	{1, 2, 2, -1, -1, 1, -1, -1, -1, -1, -1},
	{-1, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, 5, -1, 5, 5, 5, 5, 5},
	{6, 6, 6, 7, -1, 6, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, 9, -1, 8, 8, 8, 8, 8},
	{-1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, 11, 11, 11, 11, 11},
	{12, 12, 12, 12, -1, 12, 13, 13, 13, 13, 13},
	{-1, -1, -1, -1, -1, -1, 14, 17, 15, 18, 16}
	};
	int			production_idx;

	production_idx = transition_table[nonterm - 1][token->type - 1];
	if (production_idx == -1)
	{
		if (!*token->str)
			minishell_error(2, "syntax error near unexpected token `newline'");
		minishell_error(2, "syntax error near unexpected token `%s'",
			token->str);
	}
	return (production_idx);
}

#include <stdint.h>
#define OFFSET_BASIS_64  14695981039346656037UL
#define FNV_PRIME_64  1099511628211UL
#define TABLE_SIZE 100

typedef union u_types
{
	t_ast_node_type	t1;
	t_token_type	t2;
	void			*as_ptr;
	char			*as_str;
	int				as_int;
}					t_t;

typedef struct s_kv
{
	char		*k;
	t_t		v;
	struct s_kv	*n;
}				t_kv;

/**
 * @brief Calculates a 64-bit FNV-1a hash from a given input string.
 *
 * @param  *input_str: A pointer to a null-terminated string
 *                     that is to be hashed.
 *
 * @retval The 64-bit hash value as an unsigned long integer.
 */
uint64_t	fnv_1a_64(char *input_str)
{
	unsigned char	*str;
	uint64_t		hash;

	str = (unsigned char *)input_str;
	hash = OFFSET_BASIS_64;
	while (*str)
	{
		hash ^= (unsigned long)*str;
		hash *= FNV_PRIME_64;
		str++;
	}
	return (hash);
}

uint64_t	hash(char *key)
{
	return (fnv_1a_64(key));
}

void	ht_set(t_kv ht[TABLE_SIZE], char *key, t_t value)
{
	int		idx;
	t_kv	*new_kv;

	idx = hash(key) % TABLE_SIZE;
	if (!ht[idx].k)
	{
		ht[idx].k = key;
		ht[idx].v = value;
		ht[idx].n = NULL;
	}
	else
	{
		new_kv = ft_malloc(sizeof(*new_kv));
		new_kv->k = key;
		new_kv->v = value;
		new_kv->n = ht[idx].n;
		ht[idx].n = new_kv;
	}
}

t_t	ht_get(t_kv ht[TABLE_SIZE], char *key)
{
	t_kv	*kv;

	kv = &ht[hash(key) % TABLE_SIZE];
	while (kv && kv->k && ft_strcmp(kv->k, key))
		kv = kv->n;
	if (kv && kv->k)
		return (kv->v);
	return ((t_t){0});
}

/*
void	ht_print(t_kv ht[TABLE_SIZE])
{
	int		i;
	t_kv	*kv;
	int		j;

	i = -1;
	while (++i < TABLE_SIZE)
	{
		ft_printf("%d: ", i);
		kv = &ht[i];
		j = 0;
		while (1)
		{
			if (!kv || !kv->k)
				break ;
			if (j++ > 0)
				printf(" -> ");
			printf("\033[31m%s\033[m:\033[32m%s\033[m", ft_strtrim(kv->k, "\n"),
				ft_strtrim(kv->v, "\n"));
			kv = kv->n;
		}
		printf("\n");
	}
}
*/

/*
void	ht_destroy(t_kv ht[TABLE_SIZE])
{
	int		i;
	t_kv	*prev;
	t_kv	*kv;
	int		j;

	i = -1;
	while (++i < TABLE_SIZE)
	{
		kv = &ht[i];
		if (kv->k == NULL)
			continue ;
		j = 0;
		while (kv)
		{
			free(kv->k);
			free(kv->v);
			prev = kv;
			kv = kv->n;
			if (j > 0)
				free(prev);
			++j;
		}
	}
}
*/

t_ast_node_type	ast_ht_get(char *key)
{
	static t_kv	ht[TABLE_SIZE + 1];

	if (ht[TABLE_SIZE].v.as_int != 0)
		return (ht_get(ht, key).t1);
	ht_set(ht, "<pipe_sequence>", (t_t){.t1 = PIPE_SEQUENCE});
	ht_set(ht, "<complete_command_tail>", (t_t){.t1 = COMPLETE_COMMAND_TAIL});
	ht_set(ht, "<and_or>", (t_t){.t1 = AND_OR});
	ht_set(ht, "<command>", (t_t){.t1 = COMMAND});
	ht_set(ht, "<pipe_sequence_tail>", (t_t){.t1 = PIPE_SEQUENCE_TAIL});
	ht_set(ht, "<simple_command>", (t_t){.t1 = SIMPLE_COMMAND});
	ht_set(ht, "<compound_command>", (t_t){.t1 = COMPOUND_COMMAND});
	ht_set(ht, "<complete_command>", (t_t){.t1 = COMPLETE_COMMAND});
	ht_set(ht, "<io_redirect>", (t_t){.t1 = IO_REDIRECT});
	ht_set(ht, "<simple_command_tail>", (t_t){.t1 = SIMPLE_COMMAND_TAIL});
	(ht_set(ht, "(", (t_t){.t1 = TOKEN}), ht_set(ht, ")", (t_t){.t1 = TOKEN}));
	(ht_set(ht, ">", (t_t){.t1 = TOKEN}), ht_set(ht, "<", (t_t){.t1 = TOKEN}));
	ht_set(ht, "||", (t_t){.t1 = TOKEN});
	ht_set(ht, "&&", (t_t){.t1 = TOKEN});
	ht_set(ht, ">>", (t_t){.t1 = TOKEN});
	ht_set(ht, "<<", (t_t){.t1 = TOKEN});
	ht_set(ht, "TOK_WORD", (t_t){.t1 = TOKEN});
	ht_set(ht, "TOK_EPSILON", (t_t){.t1 = TOKEN});
	ht_set(ht, "|", (t_t){.t1 = TOKEN});
	ht[TABLE_SIZE].v = (t_t){.as_int = 1};
	return (ht_get(ht, key).t1);
}

t_token_type	tokens_ht_get(char *key)
{
	static t_kv	ht[TABLE_SIZE + 1];

	if (ht[TABLE_SIZE].v.as_int == 0)
	{
		ht_set(ht, "TOK_EPSILON", (t_t){.t2 = TOK_EPSILON});
		ht_set(ht, "TOK_WORD", (t_t){.t2 = TOK_WORD});
		ht_set(ht, "<<", (t_t){.t2 = TOK_HEREDOC});
		ht_set(ht, ">>", (t_t){.t2 = TOK_APPEND});
		ht_set(ht, "<", (t_t){.t2 = TOK_INPUT});
		ht_set(ht, ">", (t_t){.t2 = TOK_OVERRIDE});
		ht_set(ht, "(", (t_t){.t2 = TOK_L_PAREN});
		ht_set(ht, ")", (t_t){.t2 = TOK_R_PAREN});
		ht_set(ht, "&&", (t_t){.t2 = TOK_AND});
		ht_set(ht, "||", (t_t){.t2 = TOK_OR});
		ht_set(ht, "|", (t_t){.t2 = TOK_PIPE});
		ht[TABLE_SIZE].v = (t_t){.as_int = 1};
	}
	return (ht_get(ht, key).t2);
}

void	print_productions(t_ast_node_named_union **prods, int max_i)
{
	int						i;
	int						j;
	t_ast_node_named_union	prod;

	i = -1;
	while (++i <= max_i)
	{
		j = -1;
		while (prods[i][++j].u_d.token)
		{
			prod = prods[i][j];
			ft_printf("i:%d, j:%d\n", i, j);
			ft_printf("ast_node_type: %s\n", ast_node_type_to_string(prod.type));
			ft_printf("token_ptr: %p\n", prod.u_d.token);
			ft_printf("token_type: %s\n", token_type_to_string(prod.u_d.token->type));
			ft_printf("token_str: %s\n", prod.u_d.token->str);
		}
	}
}

t_ast_node_named_union	gen_production(char *token_str)
{
	t_token					token;
	t_ast_node_named_union	node;

	token = (t_token){\
		.type = tokens_ht_get(token_str), \
		.str = "", \
		.is_last_subtoken = TRUE \
	};
	node = (t_ast_node_named_union){
		ast_ht_get(token_str), \
		{.token = ft_memdup(&token, sizeof(token))}, \
		{0}
	};
	return (*(t_ast_node_named_union *)ft_memdup(&node, sizeof(node)));
}

t_ast_node_named_union	**initialize_productions(const char *grammar)
{
	static t_ast_node_named_union	**productions = NULL;
	char							**lines;
	char							**tokens;
	int								i;
	int								j;

	productions = ft_malloc(sizeof(*productions) * 20);
	lines = ft_split(grammar, '\n');
	i = -1;
	while (lines[++i])
	{
		tokens = ft_split(lines[i], ' ');
		j = 0;
		while (tokens[j])
			++j;
		productions[i] = ft_malloc(sizeof(*productions[i]) * (size_t)(j + 1));
		j = -1;
		while (tokens[++j])
			productions[i][j] = gen_production(tokens[j]);
		productions[i][j] = (t_ast_node_named_union){0};
	}
	return (productions);
}

/* TODO: Don't use 0 as NULL */
/* tabsize: 4 */
t_ast_node_named_union	*get_production(t_ast_node_type nonterm, t_token *token)
{
	static t_ast_node_named_union	**productions = NULL;
	static const char				*grammar = \
		"<pipe_sequence> <complete_command_tail>"			"\n"	\
		"TOK_EPSILON"										"\n"	\
		"<and_or> <pipe_sequence> <complete_command_tail>"	"\n"	\
		"&&"												"\n"	\
		"||"												"\n"	\
		"<command> <pipe_sequence_tail>"					"\n"	\
		"TOK_EPSILON"										"\n"	\
		"| <command> <pipe_sequence_tail>"					"\n"	\
		"<simple_command>"									"\n"	\
		"<compound_command>"								"\n"	\
		"( <complete_command> )"							"\n"	\
		"<io_redirect> TOK_WORD <simple_command_tail>"		"\n"	\
		"TOK_EPSILON"										"\n"	\
		"<io_redirect> TOK_WORD <simple_command_tail>"		"\n"	\
		"TOK_EPSILON"										"\n"	\
		">>" "\n"	"<<" "\n"	">" "\n"	"<" "\n"				\
		"TOK_EPSILON";

	if (productions == NULL)
		productions = initialize_productions(grammar);
	return (productions[get_production_idx(nonterm, token)]);
}

t_ast_node	*production_to_child(t_ast_node_named_union production)
{
	t_ast_node	*child;

	child = ft_malloc(sizeof(*child));
	ft_memcpy(child, &production, sizeof(production));
	if (production.type == TOKEN)
		child->token = new_token(child->token->str,
				child->token->type, TRUE);
	return (child);
}

t_deque	*productions_to_children(t_ast_node_named_union *productions)
{
	int		size;
	t_deque	*children;

	children = deque_init();
	size = 0;
	while (!ast_node_is_null((t_ast_node *)productions + size))
		deque_push_ptr_right(children,
			production_to_child(productions[size++]));
	return (children);
}

void	print_ast_node(void *data, t_bool first)
{
	t_ast_node	*node;

	node = (t_ast_node *)data;
	if (first)
	{
		if (node->type == TOKEN)
			ft_printf("%s (%s)", ast_node_type_to_string(node->type),
				token_type_to_string(node->token->type));
		else
			ft_printf("%s", ast_node_type_to_string(node->type));
	}
	else
	{
		if (node->type == TOKEN)
			ft_printf(" -> %s (%s)", ast_node_type_to_string(node->type),
				token_type_to_string(node->token->type));
		else
			ft_printf(" -> %s", ast_node_type_to_string(node->type));
	}
}

/* LL(1) parser */
t_ast_node	*build_parse_tree(t_deque *tokens)
{
	t_ast_node	*ast_node;
	t_ast_node	*ast_root_node;
	t_deque		*stack;
	t_ast_node	*top;
	t_deque		*children;

	stack = deque_init();
	deque_push_ptr_right(stack, production_to_child(\
		(t_ast_node_named_union){COMPLETE_COMMAND, {0}, {0}}));
	deque_push_ptr_right(stack, production_to_child(\
		(t_ast_node_named_union){TOKEN, {&(t_token){TOK_EOL, "", TRUE}}, {0}}));
	ast_node = new_ast_nonterm(COMPLETE_COMMAND, NULL);
	ast_root_node = ast_node;
	while (1)
	{
		top = deque_pop_left(stack)->as_ast_node;
		if (top->type != TOKEN)
		{
			children = productions_to_children(get_production(top->type, tokens->head->as_token));
			deque_extend_left(stack, deque_shallow_copy(children));
			ast_node->children = children;
			ast_node = ast_node->children->head->as_ast_node;
		}
		else
		{
			if (top->token->type == TOK_EPSILON)
			{
				ast_node = stack->head->as_ast_node;
				continue ;
			}
			else if (get_token_type(tokens) == TOK_EOL && top->token->type == TOK_EOL)
				break ;
			else if (get_token_type(tokens) == top->token->type)
			{
				ast_node->token->str = get_token_str(tokens);
				ast_node = stack->head->as_ast_node;
				deque_rotate(tokens, 1);
			}
			else if (get_token_type(tokens) == TOK_EOL)
				minishell_error(2, "syntax error near unexpected token `%s'",
					get_token_str_nl(tokens));
			else
				minishell_error(2, "syntax error near unexpected token `%s'",
					get_token_str_nl(tokens));
		}
	}
	return (ast_root_node);
}

t_deque	*build_ast_recursively(t_ast_node *node)
{
	t_deque		*children;
	t_ast_node	*head;
	t_deque		*flat;

	flat = deque_init();
	if (node->type == TOKEN)
	{
		if (node->token->type != TOK_EPSILON
			&& node->token->type != TOK_PIPE
			&& node->token->type != TOK_L_PAREN
			&& node->token->type != TOK_R_PAREN)
			deque_push_ptr_right(flat, node);
	}
	else if (node->type == IO_REDIRECT)
		assert(FALSE);
	else if (node->type == COMPLETE_COMMAND
		|| node->type == PIPE_SEQUENCE
		|| node->type == SIMPLE_COMMAND)
	{
		deque_push_ptr_right(flat, node);
		children = node->children;
		node->children = deque_init();
		while (children->size > 0)
		{
			head = deque_pop_left(children)->as_ast_node;
			if (head->type == IO_REDIRECT)
			{
				if (((t_ast_node *)head->children->head->as_ast_node)->token->type != TOK_EPSILON)
				{
					deque_push_ptr_right(node->children, head);
					head = deque_pop_left(children)->as_ast_node;
					if (head->type != TOKEN)
						assert(FALSE);
					deque_push_ptr_right(((t_ast_node *)node->children->head->prev->as_ast_node)->children, head);
				}
			}
			else
				deque_extend_right(node->children, build_ast_recursively(head));
		}
	}
	else if (node->type == COMPLETE_COMMAND_TAIL
		|| node->type == PIPE_SEQUENCE_TAIL
		|| node->type == SIMPLE_COMMAND_TAIL
		|| node->type == COMMAND
		|| node->type == COMPOUND_COMMAND
		|| node->type == AND_OR)
	{
		children = node->children;
		while (children->size > 0)
		{
			head = deque_pop_left(children)->as_ast_node;
			if (head->type == IO_REDIRECT)
			{
				if (((t_ast_node *)head->children->head->as_ast_node)->token->type != TOK_EPSILON)
				{
					deque_push_ptr_right(flat, head);
					head = deque_pop_left(children)->as_ast_node;
					if (head->type != TOKEN)
						assert(FALSE);
					deque_push_ptr_right(((t_ast_node *)flat->head->prev->as_ast_node)->children, head);
				}
			}
			else
				deque_extend_right(flat, build_ast_recursively(head));
		}
	}
	else
	{
		ft_printf("node->type: \033[34m%s\033[m\n", ast_node_type_to_string(node->type));
		assert(FALSE);
	}
	return (flat);
}

t_ast_node	*build_ast(t_deque *tokens)
{
	t_ast_node	*parse_tree;
	t_ast_node	*ast;

	parse_tree = build_parse_tree(tokens);
	ast = deque_pop_left(build_ast_recursively(parse_tree))->as_ast_node;
	/* ast = return_example_ast(); */
	return (ast);
}
