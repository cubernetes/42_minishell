
void	tree_print_with_depth(t_tree *tree, int n)
{
	t_di			*di;

	ft_putstr_n_clr("|   ", n, true);
	if (tree->type != TOKEN)
	{
		if (tree->type == SIMPLE_COMMAND)
			ft_printf("- <%s> (%d children, in: %d, out: %d)\n",
				tree_type_to_string(tree->type),
				tree->children->size,
				tree->fd_in,
				tree->fd_out);
		else
			ft_printf("- <%s> (%d children)\n",
				tree_type_to_string(tree->type),
				tree->children->size);
		di = di_begin(tree->children);
		while (di_next(di))
			tree_print_with_depth(di_get(di)->as_tree, n + 1);
	}
	else
		ft_printf("- %s (\033[31m%s\033[m)\n",
			token_type_to_string(tree->token->type),
			tree->token->str);
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

void	print_tree_node(void *data, bool first)
{
	t_tree	*tree;

	tree = (t_tree *)data;
	if (first)
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
