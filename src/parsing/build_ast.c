#include "minishell.h"
#include "libft.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* LL(1) parser */
t_tree	*build_parse_tree(t_deque *tokens)
{
	t_tree	*tree;
	t_tree	*tree_root;
	t_deque		*stack;
	t_tree	*top;
	t_deque		*children;

	stack = deque_init();
	deque_push_ptr_right(stack, production_to_child(\
		(t_tree){COMPLETE_COMMAND, {0}, {{0}}}));
	deque_push_ptr_right(stack, production_to_child(\
		(t_tree){TOKEN, {.token = &(t_token){TOK_EOL, "", true}}, {{0}}}));
	tree = new_tree_nonterm(COMPLETE_COMMAND, NULL);
	tree_root = tree;
	while (1)
	{
		top = deque_pop_left(stack)->as_tree;
		if (top->type != TOKEN)
		{
			children = productions_to_children(get_production(top->type, tokens->head->as_token));
			deque_extend_left(stack, deque_shallow_copy(children));
			tree->children = children;
			tree = tree->children->head->as_tree;
		}
		else
		{
			if (top->token->type == TOK_EPSILON)
			{
				tree = stack->head->as_tree;
				continue ;
			}
			else if (get_token_type(tokens) == TOK_EOL && top->token->type == TOK_EOL)
				break ;
			else if (get_token_type(tokens) == top->token->type)
			{
				tree->token->str = get_token_str(tokens);
				tree = stack->head->as_tree;
				deque_rotate(tokens, 1);
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

t_deque	*build_tree_recursively(t_tree *tree)
{
	t_deque		*children;
	t_tree	*head;
	t_deque		*flat;

	flat = deque_init();
	if (tree->type == TOKEN)
	{
		if (tree->token->type != TOK_EPSILON
			&& tree->token->type != TOK_PIPE
			&& tree->token->type != TOK_L_PAREN
			&& tree->token->type != TOK_R_PAREN)
			deque_push_ptr_right(flat, tree);
	}
	else if (tree->type == IO_REDIRECT)
		assert(false);
	else if (tree->type == COMPLETE_COMMAND
		|| tree->type == PIPE_SEQUENCE
		|| tree->type == SIMPLE_COMMAND)
	{
		deque_push_ptr_right(flat, tree);
		children = tree->children;
		tree->children = deque_init();
		while (children->size > 0)
		{
			head = deque_pop_left(children)->as_tree;
			if (head->type == IO_REDIRECT)
			{
				if (((t_tree *)head->children->head->as_tree)->token->type != TOK_EPSILON)
				{
					deque_push_ptr_right(tree->children, head);
					head = deque_pop_left(children)->as_tree;
					if (head->type != TOKEN)
						assert(false);
					deque_push_ptr_right(((t_tree *)tree->children->head->prev->as_tree)->children, head);
				}
			}
			else
				deque_extend_right(tree->children, build_tree_recursively(head));
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
		while (children->size > 0)
		{
			head = deque_pop_left(children)->as_tree;
			if (head->type == IO_REDIRECT)
			{
				if (((t_tree *)head->children->head->as_tree)->token->type != TOK_EPSILON)
				{
					deque_push_ptr_right(flat, head);
					head = deque_pop_left(children)->as_tree;
					if (head->type != TOKEN)
						assert(false);
					deque_push_ptr_right(((t_tree *)flat->head->prev->as_tree)->children, head);
				}
			}
			else
				deque_extend_right(flat, build_tree_recursively(head));
		}
	}
	else
	{
		ft_printf("tree->type: \033[34m%s\033[m\n", tree_type_to_string(tree->type));
		assert(false);
	}
	return (flat);
}

t_tree	*build_ast(t_deque *tokens)
{
	t_tree	*parse_tree;
	t_tree	*ast;

	parse_tree = build_parse_tree(tokens);
	ast = deque_pop_left(build_tree_recursively(parse_tree))->as_tree;
	/* ast = return_example_ast(); */
	ast->fd_in = -2;
	ast->fd_out = -2;
	ast->fd_err = -2;
	return (ast);
}
