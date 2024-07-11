/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:54:40 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/11 18:06:28 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	prepare_stack(t_list *stack[static 1])
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

bool	starts_with_bang(t_token *token)
{
	if (token->type == TOK_WORD
		&& !ft_strcmp(token->str, "!")
		&& token->expansion_ctx[0] == '0'
		&& token->quoting_ctx[0] == '0'
		&& token->escape_ctx[0] == '0')
		return (true);
	return (false);
}

bool	terminates_pipe_sequence(t_token *token)
{
	if (token->type == TOK_SEMI
		|| token->type == TOK_AND
		|| token->type == TOK_OR
		|| token->type == TOK_EOL
		|| token->type == TOK_R_PAREN)
		return (true);
	return (false);
}

bool	handle_ast_node(
	t_tree *tree[static 1],
	t_tree top[static 1],
	t_list tokens[static 1],
	t_list stack[static 1])
{
	t_list	*children;
	t_tree	*production;

	while (top->type == PIPE_SEQUENCE && starts_with_bang(
			tokens->first->as_token))
	{
		(*tree)->negated = !(*tree)->negated;
		lrotate(tokens, 1);
	}
	if (top->type == PIPE_SEQUENCE
		&& terminates_pipe_sequence(tokens->first->as_token))
		lpush_left(tokens, as_token(new_token(":", TOK_WORD, true)));
	else if (top->type == PIPE_SEQUENCE_TAIL && starts_with_bang(tokens->first
			->next->as_token) && tokens->first->as_token->type != TOK_EOL)
		return (set_last_exit_status(minishell_error(2,
					mk_err_flags(!option_enabled('i'), true),
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

int	handle_token(
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
					mk_err_flags(!option_enabled('i'), true),
					"syntax error near unexpected token `%s'",
					get_token_str_nl(tokens))), 3);
	else
		return (set_last_exit_status(minishell_error(2,
					mk_err_flags(!option_enabled('i'), true),
					"syntax error near unexpected token `%s'",
					get_token_str_nl(tokens))), 3);
	return (0);
}
