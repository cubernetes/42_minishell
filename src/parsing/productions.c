#include "libft.h"
#include "minishell.h"

t_tree	gen_production(char *token_str)
{
	return (*(t_tree *)ft_memdup(\
		&(t_tree){
			.type = tree_ht_get(token_str),
			.token = ft_memdup(
				&(t_token){\
					.type = tokens_ht_get(token_str), \
					.str = "", \
					.is_last_subtoken = true \
				}, \
				sizeof(t_token) \
			), \
			{{0}} \
		},
		sizeof(t_tree)
	));
}

t_tree	**initialize_productions(const char *grammar)
{
	t_tree	**productions;
	char		**lines;
	char		**tokens;
	int			i;
	int			j;

	gc_set_context("POST");
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
		productions[i][j] = (t_tree){0};
	}
	gc_set_context("DEFAULT");
	return (productions);
}

/* TODO: Don't use 0 as NULL */
/* tabsize: 4 */
t_tree	*get_production(t_tree_type nonterm, t_token *token)
{
	static t_tree		**productions = NULL;
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

	if (productions == NULL)
		productions = initialize_productions(grammar);
	return (productions[get_production_idx(nonterm, token)]);
}

t_tree	*production_to_child(t_tree production)
{
	t_tree	*child;

	child = ft_malloc(sizeof(*child));
	ft_memcpy(child, &production, sizeof(production));
	if (production.type == TOKEN)
		child->token = new_token(child->token->str,
				child->token->type, true);
	return (child);
}

t_list	*productions_to_children(t_tree *productions)
{
	int		size;
	t_list	*children;

	children = lnew();
	size = 0;
	while (!tree_is_null((t_tree *)productions + size))
		lpush(children,
			as_tree(production_to_child(productions[size++])));
	return (children);
}
