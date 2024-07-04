/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 16:01:55 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 18:59:08 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft.h"

#include <stdlib.h>
#include <stdbool.h>

void	_prepare_stack(t_list *stack[static 1]);
bool	_starts_with_bang(t_list *tokens);
bool	_terminates_pipe_sequence(t_list *tokens);
bool	_handle_ast_node(
			t_tree *tree[static 1],
			t_tree top[static 1],
			t_list tokens[static 1],
			t_list stack[static 1]);
int		_handle_token(
			t_tree *tree[static 1],
			t_tree top[static 1],
			t_list tokens[static 1],
			t_list stack[static 1]);
t_list	*build_as_recursively(t_tree *tree, bool mk_heredoc);
bool	_is_unimportant_token(t_token *token);
bool	_is_proper_ast_node(t_tree *tree);
bool	_is_improper_ast_node(t_tree *tree);
void	_flatten_redirect(t_tree *first, t_list *chldn, t_list *children,
			bool mk_heredoc);
void	_handle_proper_ast_node(t_tree *tree, t_list *flat, bool mk_heredoc);

/* LL(1) parser */
static t_tree	*build_parse_tree(t_list tokens[static 1])
{
	t_tree	*tree;
	t_tree	*top;
	t_list	*stack;
	t_tree	*tree_root;
	int		op;

	tree = new_tree_nonterm(COMPLETE_COMMAND, NULL);
	(_prepare_stack(&stack), tree_root = tree);
	while (true)
	{
		top = lpop_left(stack)->as_tree;
		if (top->type == TOKEN)
		{
			op = _handle_token(&tree, top, tokens, stack);
			if (op == 1)
				continue ;
			else if (op == 2)
				break ;
			else if (op == 3)
				return (NULL);
		}
		else if (_handle_ast_node(&tree, top, tokens, stack) == false)
			return (NULL);
	}
	return (tree_root);
}

static void	_handle_improper_ast_node(t_tree *tree, t_list *flat,
	bool mk_heredoc)
{
	t_list	*children;
	t_tree	*first;

	children = tree->children;
	while (children->len > 0)
	{
		first = lpop_left(children)->as_tree;
		if (first->type != IO_REDIRECT)
			lextend(flat, build_as_recursively(first, mk_heredoc));
		else if (first->children->first->as_tree->token->type != TOK_EPSILON)
			_flatten_redirect(first, flat, children, mk_heredoc);
	}
}

t_list	*build_as_recursively(t_tree *tree, bool mk_heredoc)
{
	t_list	*flat;

	flat = lnew();
	if (_is_proper_ast_node(tree))
		_handle_proper_ast_node(tree, flat, mk_heredoc);
	else if (_is_improper_ast_node(tree))
		_handle_improper_ast_node(tree, flat, mk_heredoc);
	if (tree->type == TOKEN && _is_unimportant_token(tree->token))
		(void)lpush(flat, as_tree(tree));
	return (flat);
}

t_tree	*build_ast(t_list *tokens, bool mk_heredoc)
{
	t_tree	*parse_tree;
	t_tree	*ast;

	if (tokens == NULL)
		return (NULL);
	parse_tree = build_parse_tree(tokens);
	if (parse_tree == NULL)
		return (NULL);
	heredoc_aborted(0);
	ast = lpop_left(build_as_recursively(parse_tree, mk_heredoc))->as_tree;
	ast->fd_in = -2;
	ast->fd_out = -2;
	ast->fd_err = -2;
	return (ast);
}
