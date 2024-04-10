#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <stdlib.h>
#include <assert.h>

/* TODO: Not required: hashtable */
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
	if (!ft_memcmp(ast_node, &(t_ast_node){0}, sizeof(t_ast_node)))
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
	return (ft_memdup(\
		&(t_ast_node){
			.type = TOKEN,
			{.token = new_token(str, type, TRUE)},
			{{{0}, 0}}
		},
		sizeof(t_ast_node)
	));
}

t_ast_node	*new_ast_m_token(t_token_type type, const char *str)
{
	return (new_ast_token(type, ft_strdup(str)));
}

/* children must be allocated on heap: Update, maybe outdated */
t_ast_node	*new_ast_nonterm(t_ast_node_type type, t_deque *children)
{
	return (ft_memdup(\
		&(t_ast_node){
			.type = type,
			.children = children
		},
		sizeof(t_ast_node)
	));
}

void	ast_print_with_depth(t_ast_node *ast_node, int n)
{
	t_di			*di;

	repeat_string("|   ", n, TRUE);
	if (ast_node->type != TOKEN)
	{
		if (ast_node->type == SIMPLE_COMMAND)
			ft_printf("- <%s> (%d children, in: %d, out: %d, exitcode: %d)\n",
				ast_node_type_to_string(ast_node->type),
				ast_node->children->size,
				ast_node->simple_cmd_meta.fds.in,
				ast_node->simple_cmd_meta.fds.out,
				ast_node->simple_cmd_meta.exit_status);
		else
			ft_printf("- <%s> (%d children)\n",
				ast_node_type_to_string(ast_node->type),
				ast_node->children->size);
		di = di_begin(ast_node->children);
		while (di_next(di))
			ast_print_with_depth(di_get(di)->as_ast_node, n + 1);
	}
	else
		ft_printf("- %s (\033[31m%s\033[m)\n",
			token_type_to_string(ast_node->token->type),
			ast_node->token->str);
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
/* TODO: Not required: Make transition table adaptive (quite a lot of work) */
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
			minishell_error(2, TRUE,
				"syntax error near unexpected token `newline'");
		minishell_error(2, TRUE, "syntax error near unexpected token `%s'",
			token->str);
	}
	return (production_idx);
}

t_type	ht_tok(t_token_type token_type)
{
	return ((t_type){
		.t2 = token_type
	});
}

t_type	ht_ast(t_ast_node_type ast_node_type)
{
	return ((t_type){
		.t1 = ast_node_type
	});
}

t_ast_node_type	ast_ht_get(char *key)
{
	static t_kv	ht[TABLE_SIZE + 1];

	manage_static_ptr(&ht[TABLE_SIZE].v.as_ptr);
	if (ht[TABLE_SIZE].v.as_ptr != NULL)
		return (ht_get(ht, key).t1);
	ht_set(ht, "<pipe_sequence>", ht_ast(PIPE_SEQUENCE));
	ht_set(ht, "<complete_command_tail>", ht_ast(COMPLETE_COMMAND_TAIL));
	ht_set(ht, "<and_or>", ht_ast(AND_OR));
	ht_set(ht, "<command>", ht_ast(COMMAND));
	ht_set(ht, "<pipe_sequence_tail>", ht_ast(PIPE_SEQUENCE_TAIL));
	ht_set(ht, "<simple_command>", ht_ast(SIMPLE_COMMAND));
	ht_set(ht, "<compound_command>", ht_ast(COMPOUND_COMMAND));
	ht_set(ht, "<complete_command>", ht_ast(COMPLETE_COMMAND));
	ht_set(ht, "<io_redirect>", ht_ast(IO_REDIRECT));
	ht_set(ht, "<simple_command_tail>", ht_ast(SIMPLE_COMMAND_TAIL));
	(ht_set(ht, "(", ht_ast(TOKEN)), ht_set(ht, ")", ht_ast(TOKEN)));
	(ht_set(ht, ">", ht_ast(TOKEN)), ht_set(ht, "<", ht_ast(TOKEN)));
	(ht_set(ht, "||", ht_ast(TOKEN)), ht_set(ht, "&&", ht_ast(TOKEN)));
	ht_set(ht, ">>", ht_ast(TOKEN));
	ht_set(ht, "<<", ht_ast(TOKEN));
	ht_set(ht, "TOK_WORD", ht_ast(TOKEN));
	ht_set(ht, "TOK_EPSILON", ht_ast(TOKEN));
	ht_set(ht, "|", ht_ast(TOKEN));
	ht[TABLE_SIZE].v.as_ptr = (void *)1;
	return (ht_get(ht, key).t1);
}

t_token_type	tokens_ht_get(char *key)
{
	static t_kv	ht[TABLE_SIZE + 1];

	manage_static_ptr(&ht[TABLE_SIZE].v.as_ptr);
	if (ht[TABLE_SIZE].v.as_ptr == NULL)
	{
		ht_set(ht, "TOK_EPSILON", ht_tok(TOK_EPSILON));
		ht_set(ht, "TOK_WORD", ht_tok(TOK_WORD));
		ht_set(ht, "<<", ht_tok(TOK_HEREDOC));
		ht_set(ht, ">>", ht_tok(TOK_APPEND));
		ht_set(ht, "<", ht_tok(TOK_INPUT));
		ht_set(ht, ">", ht_tok(TOK_OVERRIDE));
		ht_set(ht, "(", ht_tok(TOK_L_PAREN));
		ht_set(ht, ")", ht_tok(TOK_R_PAREN));
		ht_set(ht, "&&", ht_tok(TOK_AND));
		ht_set(ht, "||", ht_tok(TOK_OR));
		ht_set(ht, "|", ht_tok(TOK_PIPE));
		ht[TABLE_SIZE].v.as_ptr = (void *)1;
	}
	return (ht_get(ht, key).t2);
}

void	print_productions(t_ast_node **prods, int max_i)
{
	int			i;
	int			j;
	t_ast_node	prod;

	i = -1;
	while (++i <= max_i)
	{
		j = -1;
		while (prods[i][++j].token)
		{
			prod = prods[i][j];
			ft_printf("i:%d, j:%d\n", i, j);
			ft_printf("ast_node_type: %s\n", ast_node_type_to_string(prod.type));
			ft_printf("token_ptr: %p\n", prod.token);
			ft_printf("token_type: %s\n", token_type_to_string(prod.token->type));
			ft_printf("token_str: %s\n", prod.token->str);
		}
	}
}

t_ast_node	gen_production(char *token_str)
{
	return (*(t_ast_node *)ft_memdup(\
		&(t_ast_node){
			.type = ast_ht_get(token_str),
			.token = ft_memdup(
				&(t_token){\
					.type = tokens_ht_get(token_str), \
					.str = "", \
					.is_last_subtoken = TRUE \
				}, \
				sizeof(t_token) \
			), \
			{{{0}, 0}} \
		},
		sizeof(t_ast_node)
	));
}

t_ast_node	**initialize_productions(const char *grammar)
{
	t_ast_node	**productions;
	char		**lines;
	char		**tokens;
	int			i;
	int			j;

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
		productions[i][j] = (t_ast_node){0};
	}
	return (productions);
}

/* TODO: Don't use 0 as NULL */
/* tabsize: 4 */
t_ast_node	*get_production(t_ast_node_type nonterm, t_token *token)
{
	static t_ast_node	**productions = NULL;
	static const char	*grammar = \
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

	if (productions == NULL && (manage_static_ptr((void **)&productions), 1))
		productions = initialize_productions(grammar);
	return (productions[get_production_idx(nonterm, token)]);
}

t_ast_node	*production_to_child(t_ast_node production)
{
	t_ast_node	*child;

	child = ft_malloc(sizeof(*child));
	ft_memcpy(child, &production, sizeof(production));
	if (production.type == TOKEN)
		child->token = new_token(child->token->str,
				child->token->type, TRUE);
	return (child);
}

t_deque	*productions_to_children(t_ast_node *productions)
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
		(t_ast_node){COMPLETE_COMMAND, {0}, {{{0}, 0}}}));
	deque_push_ptr_right(stack, production_to_child(\
		(t_ast_node){TOKEN, {.token = &(t_token){TOK_EOL, "", TRUE}}, {{{0}, 0}}}));
	ast_node = new_ast_nonterm(COMPLETE_COMMAND, NULL);
	ast_root_node = ast_node;
	/* ft_printf("\n"); */
	/* ft_printf("START\n"); */
	while (1)
	{
		/* deque_print(stack, print_ast_node); */
		/* deque_print(tokens, print_token); */
		/* ft_printf("\n"); */
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
				minishell_error(2, TRUE,
					"syntax error near unexpected token `%s'",
					get_token_str_nl(tokens));
			else
				minishell_error(2, TRUE,
					"syntax error near unexpected token `%s'",
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
