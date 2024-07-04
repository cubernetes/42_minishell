/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 16:01:55 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 17:28:10 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

static void	_prepare_stack(t_list *stack[static 1])
{
	*stack = lnew();
	lpush(*stack, as_tree(production_part_to_child((t_tree){\
		COMPLETE_COMMAND, \
		{0}, \
		0, \
		false, \
		{{0}} \
	})));
	lpush(*stack, as_tree(production_part_to_child((t_tree){\
		TOKEN, \
		{.token = &(t_token){TOK_EOL, \
			"", true, \
			"", "", \
			0, "0", \
			""} \
		}, \
		0, \
		false, \
		{{0}} \
	})));
}

static bool	_starts_with_bang(t_list *tokens)
{
	if (tokens->first->as_token->type == TOK_WORD
		&& !ft_strcmp(tokens->first->as_token->str, "!")
		&& tokens->first->as_token->expansion_ctx[0] == '0'
		&& tokens->first->as_token->quoting_ctx[0] == '0'
		&& tokens->first->as_token->escape_ctx[0] == '0')
		return (true);
	return (false);
}

static bool	_terminates_pipe_sequence(t_list *tokens)
{
	if (tokens->first->as_token->type == TOK_SEMI
		|| tokens->first->as_token->type == TOK_AND
		|| tokens->first->as_token->type == TOK_OR
		|| tokens->first->as_token->type == TOK_EOL
		|| tokens->first->as_token->type == TOK_R_PAREN)
		return (true);
	return (false);
}

static bool	_handle_ast_node(
	t_tree *tree[static 1],
	t_tree top[static 1],
	t_list tokens[static 1],
	t_list stack[static 1])
{
	t_list	*children;
	t_tree	*production;

	while (top->type == PIPE_SEQUENCE && _starts_with_bang(tokens))
	{
		(*tree)->negated = !(*tree)->negated;
		lrotate(tokens, 1);
	}
	if (top->type == PIPE_SEQUENCE
		&& _terminates_pipe_sequence(tokens))
		lpush_left(tokens, as_token(new_token(":", TOK_WORD, true)));
	else if (top->type != PIPE_SEQUENCE && top->type != COMPLETE_COMMAND
		&& _starts_with_bang(tokens))
		return (set_last_exit_status(minishell_error(2,
					!option_enabled('i'), true,
					"syntax error near unexpected token `!'")), false);
	production = get_production(top->type, tokens->first->as_token);
	if (production == NULL)
		return (false);
	children = production_to_children(production);
	lextend_left(stack, lcopy(children));
	(*tree)->children = children;
	*tree = (*tree)->children->first->as_tree;
	return (true);
}

static int	_handle_token(
	t_tree *tree[static 1],
	t_tree top[static 1],
	t_list tokens[static 1],
	t_list stack[static 1])
{
	if (top->token->type == TOK_EPSILON)
	{
		*tree = stack->first->as_tree;
		return (1);
	}
	else if (get_token_type(tokens) == TOK_EOL && top->token->type == TOK_EOL)
		return (2);
	else if (get_token_type(tokens) == top->token->type)
	{
		(*tree)->token = tokens->first->as_token;
		*tree = stack->first->as_tree;
		lrotate(tokens, 1);
	}
	else if (get_token_type(tokens) == TOK_EOL)
		return (set_last_exit_status(minishell_error(2,
					!option_enabled('i'), true,
					"syntax error near unexpected token `%s'",
					get_token_str_nl(tokens))), 3);
	else
		return (set_last_exit_status(minishell_error(2,
					!option_enabled('i'), true,
					"syntax error near unexpected token `%s'",
					get_token_str_nl(tokens))), 3);
	return (0);
}

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

static bool	_is_unimportant_token(t_token *token)
{
	if (token->type != TOK_EPSILON
		&& token->type != TOK_PIPE
		&& token->type != TOK_L_PAREN
		&& token->type != TOK_R_PAREN)
		return (true);
	return (false);
}

static bool	_is_proper_ast_node(t_tree *tree)
{
	if (tree->type == COMPLETE_COMMAND
		|| tree->type == PIPE_SEQUENCE
		|| tree->type == SIMPLE_COMMAND)
		return (true);
	return (false);
}

static bool	_is_improper_ast_node(t_tree *tree)
{
	if (tree->type == COMPLETE_COMMAND_TAIL
		|| tree->type == PIPE_SEQUENCE_TAIL
		|| tree->type == SIMPLE_COMMAND_TAIL
		|| tree->type == COMMAND
		|| tree->type == COMPOUND_COMMAND
		|| tree->type == AND_OR)
		return (true);
	return (false);
}

t_list	*build_as_recursively(t_tree *tree, bool mk_heredoc);

static void	_flatten_redirect(t_tree *first, t_list *chldn, t_list *children,
	bool mk_heredoc)
{
	mk_heredoc &= first->children->first->as_tree->token->type == TOK_HEREDOC;
	(void)lpush(chldn, as_tree(first));
	first = lpop_left(children)->as_tree;
	if (first->type != TOKEN)
		assert(false);
	if (mk_heredoc)
		first->token->str = create_heredoc(first->token->str);
	(void)lpush(chldn->last->as_tree->children, as_tree(first));
}

static void	_handle_proper_ast_node(t_tree *tree, t_list *flat, bool mk_heredoc)
{
	t_list	*children;
	t_tree	*first;

	(void)lpush(flat, as_tree(tree));
	children = tree->children;
	tree->children = lnew();
	while (children->len > 0)
	{
		first = lpop_left(children)->as_tree;
		if (first->type != IO_REDIRECT)
			lextend(tree->children, build_as_recursively(first, mk_heredoc));
		else if (first->children->first->as_tree->token->type != TOK_EPSILON)
			_flatten_redirect(first, tree->children, children, mk_heredoc);
	}
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
