#include "minishell.h"
#include "libft.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* LL(1) parser */
t_tree	*build_parse_tree(t_list *tokens)
{
	t_tree	*tree;
	t_tree	*tree_root;
	t_list		*stack;
	t_tree	*top;
	t_list		*children;

	stack = lnew();
	lpush(stack, as_tree(production_to_child(\
		(t_tree){COMPLETE_COMMAND, {0}, {{0}}})));
	lpush(stack, as_tree(production_to_child(\
		(t_tree){TOKEN, {.token = &(t_token){TOK_EOL, "", true}}, {{0}}})));
	tree = new_tree_nonterm(COMPLETE_COMMAND, NULL);
	tree_root = tree;
	while (1)
	{
		top = stack->first->as_tree;
		lpop_left(stack);
		if (top->type != TOKEN)
		{
			children = productions_to_children(get_production(top->type, tokens->first->as_token));
			lextend_left(stack, lcopy(children));
			tree->children = children;
			tree = tree->children->first->as_tree;
		}
		else
		{
			if (top->token->type == TOK_EPSILON)
			{
				tree = stack->first->as_tree;
				continue ;
			}
			else if (get_token_type(tokens) == TOK_EOL && top->token->type == TOK_EOL)
				break ;
			else if (get_token_type(tokens) == top->token->type)
			{
				tree->token->str = get_token_str(tokens);
				tree = stack->first->as_tree;
				lrotate(tokens, 1);
			}
			else if (get_token_type(tokens) == TOK_EOL)
				minishell_error(2, true,
					"syntax error near unexpected token `%s'",
					get_token_str_nl(tokens));
			else
				minishell_error(2, true,
					"syntax error near unexpected token `%s'",
					get_token_str_nl(tokens));
		}
	}
	return (tree_root);
}

t_list	*build_tree_recursively(t_tree *tree)
{
	t_list	*children;
	t_tree	*first;
	t_list	*flat;

	flat = lnew();
	if (tree->type == TOKEN)
	{
		if (tree->token->type != TOK_EPSILON
			&& tree->token->type != TOK_PIPE
			&& tree->token->type != TOK_L_PAREN
			&& tree->token->type != TOK_R_PAREN)
			lpush(flat, as_tree(tree));
	}
	else if (tree->type == IO_REDIRECT)
		assert(false);
	else if (tree->type == COMPLETE_COMMAND
		|| tree->type == PIPE_SEQUENCE
		|| tree->type == SIMPLE_COMMAND)
	{
		lpush(flat, as_tree(tree));
		children = tree->children;
		tree->children = lnew();
		while (children->len > 0)
		{
			first = children->first->as_tree;
			lpop_left(children);
			if (first->type == IO_REDIRECT)
			{
				if (((t_tree *)first->children->first->as_tree)->token->type != TOK_EPSILON)
				{
					lpush(tree->children, as_tree(first));
					first = children->first->as_tree;
					lpop_left(children);
					if (first->type != TOKEN)
						assert(false);
					lpush(((t_tree *)tree->children->first->prev->as_tree)->children, as_tree(first));
				}
			}
			else
				lextend(tree->children, build_tree_recursively(first));
		}
	}
	else if (tree->type == COMPLETE_COMMAND_TAIL
		|| tree->type == PIPE_SEQUENCE_TAIL
		|| tree->type == SIMPLE_COMMAND_TAIL
		|| tree->type == COMMAND
		|| tree->type == COMPOUND_COMMAND
		|| tree->type == AND_OR)
	{
		children = tree->children;
		while (children->len > 0)
		{
			first = children->first->as_tree;
			lpop_left(children);
			if (first->type == IO_REDIRECT)
			{
				if (((t_tree *)first->children->first->as_tree)->token->type != TOK_EPSILON)
				{
					lpush(flat, as_tree(first));
					first = children->first->as_tree;
					lpop_left(children);
					if (first->type != TOKEN)
						assert(false);
					lpush(((t_tree *)flat->first->prev->as_tree)->children, as_tree(first));
				}
			}
			else
				lextend(flat, build_tree_recursively(first));
		}
	}
	else
	{
		ft_printf("tree->type: \033[34m%s\033[m\n", tree_type_to_string(tree->type));
		assert(false);
	}
	return (flat);
}

t_tree	*build_ast(t_list *tokens)
{
	t_tree	*parse_tree;
	t_tree	*ast;
	t_list	*tmp;

	parse_tree = build_parse_tree(tokens);
	tmp = build_tree_recursively(parse_tree);
	ast = tmp->first->as_tree;
	lpop_left(tmp);
	/* ast = return_example_ast(); */
	ast->fd_in = -2;
	ast->fd_out = -2;
	ast->fd_err = -2;
	return (ast);
}
