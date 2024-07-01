#include "minishell.h"
#include <stdbool.h>

void	tree_print_with_depth(t_tree *tree, int n)
{
	ft_putstr_n_clr("|   ", n, true);
	if (tree->type != TOKEN)
	{
		if (tree->type == SIMPLE_COMMAND)
			ft_printf("- <%s> (%d children, in: %d, out: %d, err: %d)\n",
				tree_type_to_string(tree->type),
				tree->children->len,
				tree->fd_in,
				tree->fd_out,
				tree->fd_err);
		else if (tree->type == PIPE_SEQUENCE)
			ft_printf("- <%s> (%d children, negated: %s)\n",
				tree_type_to_string(tree->type),
				tree->children->len,
				(char *[]){"false", "true"}[tree->negated]);
		else
			ft_printf("- <%s> (%d children)\n",
				tree_type_to_string(tree->type),
				tree->children->len);
		liter(tree->children);
		while (lnext(tree->children))
			tree_print_with_depth(tree->children->current->as_tree, n + 1);
	}
	else
		ft_printf("- %s (\033[31m%s\033[m, splitnum: %d, is_last: %s, origin: %s)\n",
			token_type_to_string(tree->token->type),
			tree->token->str,
			tree->token->num_tokens_after_split,
			(char *[]){"false", "true"}[tree->token->is_last_token],
			tree->token->origin
			);
}

void	tree_print(t_tree *tree)
{
	tree_print_with_depth(tree, 0);
}

void	print_productions(t_tree **prods, int max_i)
{
	int			i;
	int			j;
	t_tree	prod;

	i = -1;
	while (++i <= max_i)
	{
		j = -1;
		while (prods[i][++j].token)
		{
			prod = prods[i][j];
			ft_printf("i:%d, j:%d\n", i, j);
			ft_printf("tree_type: %s\n", tree_type_to_string(prod.type));
			ft_printf("token_ptr: %p\n", prod.token);
			ft_printf("token_type: %s\n", token_type_to_string(prod.token->type));
			ft_printf("token_str: %s\n", prod.token->str);
		}
	}
}

void	print_tree_node(void *data, int n)
{
	t_tree	*tree;

	tree = (t_tree *)data;
	if (n == 0)
	{
		if (tree->type == TOKEN)
			ft_printf("%s (%s)", tree_type_to_string(tree->type),
				token_type_to_string(tree->token->type));
		else
			ft_printf("%s", tree_type_to_string(tree->type));
	}
	else
	{
		if (tree->type == TOKEN)
			ft_printf(" -> %s (%s)", tree_type_to_string(tree->type),
				token_type_to_string(tree->token->type));
		else
			ft_printf(" -> %s", tree_type_to_string(tree->type));
	}
}
