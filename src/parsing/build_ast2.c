/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:57:09 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 18:58:16 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_list	*build_as_recursively(t_tree *tree, bool mk_heredoc);

bool	_is_unimportant_token(t_token *token)
{
	if (token->type != TOK_EPSILON
		&& token->type != TOK_PIPE
		&& token->type != TOK_L_PAREN
		&& token->type != TOK_R_PAREN)
		return (true);
	return (false);
}

bool	_is_proper_ast_node(t_tree *tree)
{
	if (tree->type == COMPLETE_COMMAND
		|| tree->type == PIPE_SEQUENCE
		|| tree->type == SIMPLE_COMMAND)
		return (true);
	return (false);
}

bool	_is_improper_ast_node(t_tree *tree)
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

void	_flatten_redirect(t_tree *first, t_list *chldn, t_list *children,
	bool mk_heredoc)
{
	mk_heredoc &= first->children->first->as_tree->token->type == TOK_HEREDOC;
	(void)lpush(chldn, as_tree(first));
	first = lpop_left(children)->as_tree;
	if (mk_heredoc)
		first->token->str = create_heredoc(first->token->str);
	(void)lpush(chldn->last->as_tree->children, as_tree(first));
}

void	_handle_proper_ast_node(t_tree *tree, t_list *flat, bool mk_heredoc)
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
