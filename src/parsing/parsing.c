/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 07:40:13 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/01 09:49:46 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenization.h"
#include <stdlib.h>

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

t_bool	ast_free(t_ast_node *ast_node)
{
	t_ast_node	**orig_children;

	if (!ast_node)
		return (TRUE);
	if (ast_node->type == TOKEN)
	{
		((void)free_token(ast_node->data.token), ast_node->data.token = NULL);
		(free(ast_node), ast_node = NULL);
		return (TRUE);
	}
	orig_children = ast_node->data.children;
	while (*ast_node->data.children)
		(void)ast_free(*ast_node->data.children++);
	(free(orig_children), ast_node->data.children = NULL);
	(free(ast_node), ast_node = NULL);
	return (TRUE);
}

void	print_production(void *data, t_bool first)
{
	t_production	*production;
	const char		*second;

	production = (t_production *)data;
	if (first)
		second = "";
	else
		second = ", ";
	if (!production)
	{
		return ;
	}
	if (production->type == TOKEN)
		ft_printf("%s\033[32m%s\033[m (\033[31m%s\033[m)", second,
			token_type_to_string(production->data.token->type),
			production->data.token->str);
	else
		ft_printf("%s\033[32m%s\033[m", second, ast_node_type_to_string(production->type));
}

void	print_children(t_ast_node **children)
{
	while (!ast_node_is_null(*children))
	{
		ft_printf("[[%s]], ", ast_node_type_to_string((*children)->type));
		++children;
	}
	ft_printf("\n");
}

void	push_productions(t_ast_node *ast_node, t_ddeque *stack, t_production **productions)
{
	if (ast_node_is_null(*productions))
		return ;
	push_productions(ast_node, stack, productions + 1);
	ddeque_push_value_top(stack, *productions);
}

void	repeat_string(const char *str, size_t n)
{
	while (n--)
		ft_printf("%s", str);
}

t_ast_node	*new_ast_node(t_ast_node_type type, t_ast_node_data data)
{
	t_ast_node	*ast_node;

	ast_node = ft_malloc(sizeof(*ast_node));
	ast_node->type = type;
	ast_node->data = data;
	return (ast_node);
}

t_ast_node	*new_ast_token(t_token_type type, char *str)
{
	t_ast_node	*ast_node;

	ast_node = ft_malloc(sizeof(*ast_node));
	ast_node->type = TOKEN;
	ast_node->data.token = new_token(str, type);
	return (ast_node);
}

t_ast_node	*new_ast_m_token(t_token_type type, const char *str)
{
	return (new_ast_token(type, ft_strdup(str)));
}

/* children must be allocated on heap */
t_ast_node	*new_ast_nonterm(t_ast_node_type type, t_ast_node **children)
{
	t_ast_node	*ast_node;

	ast_node = ft_malloc(sizeof(*ast_node));
	ast_node->type = type;
	ast_node->data.children = children;
	return (ast_node);
}

t_bool	ast_node_is_null(t_ast_node *ast_node)
{
	if (!ast_node)
		return (TRUE);
	if (ast_node->type == 0)
		if (ast_node->data.token == NULL && ast_node->data.children == NULL)
			return (TRUE);
	return (FALSE);
}

void	ast_print_with_depth(t_ast_node *ast_node, size_t n)
{
	t_ast_node	**tmp_children;

	if (!ast_node)
		internal_error("ast_print_with_depth: ast_node is NULL", __LINE__);
	repeat_string("    ", n);
	if (ast_node->type != TOKEN)
	{
		ft_printf("- <%s>\n", ast_node_type_to_string(ast_node->type));
		if (ast_node->data.children)
		{
		}
		tmp_children = ast_node->data.children;
		while (tmp_children && *tmp_children && !ast_node_is_null(*tmp_children))
		{
			ast_print_with_depth(*tmp_children, n + 1);
			++tmp_children;
		}
	}
	else
	{
		ft_printf("- %s (\033[31m%s\033[m)\n",
			token_type_to_string(ast_node->data.token->type),
			ast_node->data.token->str);
	}
}

void	ast_print(t_ast_node *ast_node)
{
	ast_print_with_depth(ast_node, 0);
}

t_ast_node	**new_children(t_ast_node **children)
{
	t_ast_node	**new_children;
	size_t		i;

	i = 0;
	while (children[i])
		++i;
	new_children = ft_malloc(sizeof(*new_children) * (++i));
	while (i--)
		new_children[i] = children[i];
	return (new_children);
}

t_ast_node	*return_example_ast(void)
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

/* return the index of the production to use */
/* TODO: fix error handling */
int	get_production_idx(t_ast_node_type nonterm, t_token *token)
{
	static int	transition_table[NUM_NONTERMS][NUM_TOKENS] = {
	{-1, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0},
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
		ft_printf("minishell: syntax error near unexpected token `%s'\n", token->str);
		internal_error("get_production_idx: Syntax error", __LINE__);
	}
	return (production_idx);
}

/* TODO: Refactor */
/* TODO: Don't use 0 as NULL */
t_production	*get_production(t_ast_node_type nonterm, t_token *token)
{
	static t_token		tokens[] = {{0}, {TOK_EOL, 0}, {TOK_AND, 0}, \
		{TOK_OR, 0}, {TOK_PIPE, 0}, {TOK_L_PAREN, 0}, {TOK_R_PAREN, 0}, \
		{TOK_WORD, 0}, {TOK_OVERRIDE, 0}, {TOK_APPEND, 0}, {TOK_INPUT, 0}, \
		{TOK_HEREDOC, 0}, {TOK_EPSILON, 0}, {TOK_SQUOTE_STR, 0}, \
		{TOK_DQUOTE_STR, 0}, {TOK_ERROR, 0}};
	static t_production	productions[][4] = {\
		{{PIPE_SEQUENCE, {0}}, {COMPLETE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tokens[TOK_EPSILON]}}, {0}}, \
		{{AND_OR, {0}}, {PIPE_SEQUENCE, {0}}, {COMPLETE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tokens[TOK_AND]}}, {0}}, {{TOKEN, {&tokens[TOK_OR]}}, {0}}, \
		{{COMMAND, {0}}, {PIPE_SEQUENCE_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tokens[TOK_EPSILON]}}, {0}}, \
		{{TOKEN, {&tokens[TOK_PIPE]}}, {COMMAND, {0}}, {PIPE_SEQUENCE_TAIL, {0}}, {0}}, \
		{{SIMPLE_COMMAND, {0}}, {0}}, {{COMPOUND_COMMAND, {0}}, {0}}, \
		{{TOKEN, {&tokens[TOK_L_PAREN]}}, {COMPLETE_COMMAND, {0}}, {TOKEN, {&tokens[TOK_R_PAREN]}}, {0}}, \
		{{IO_REDIRECT, {0}}, {TOKEN, {&tokens[TOK_WORD]}}, {SIMPLE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tokens[TOK_EPSILON]}}, {0}}, \
		{{IO_REDIRECT, {0}}, {TOKEN, {&tokens[TOK_WORD]}}, {SIMPLE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tokens[TOK_EPSILON]}}, {0}}, \
		{{TOKEN, {&tokens[TOK_APPEND]}}, {0}}, \
		{{TOKEN, {&tokens[TOK_HEREDOC]}}, {0}}, \
		{{TOKEN, {&tokens[TOK_OVERRIDE]}}, {0}}, \
		{{TOKEN, {&tokens[TOK_INPUT]}}, {0}}};

	return (productions[get_production_idx(nonterm, token)]);
}

t_ast_node	*production_to_child(t_production prodcution)
{
	t_ast_node	*child;

	child = ft_malloc(sizeof(*child));
	if (sizeof(*child) != sizeof(t_production))
		internal_error("production_to_child: different sizes", __LINE__);
	ft_memcpy(child, &prodcution, sizeof(prodcution));
	if (prodcution.type == TOKEN)
		child->data.token = new_token(child->data.token->str, child->data.token->type);
	return (child);
}

t_ast_node	**productions_to_children(t_production *productions)
{
	int			size;
	t_ast_node	**children;

	size = 0;
	while (!ast_node_is_null(productions + size))
		++size;
	children = ft_malloc(sizeof(*children) * (size_t)(size + 1));
	size = 0;
	while (!ast_node_is_null(productions + size))
	{
		children[size] = production_to_child(productions[size]);
		++size;
	}
	children[size] = NULL;
	return (children);
}

/* LL(1) parser */
/* TODO: improve error handling */
t_ast_node	*build_ast(t_ddeque *tokens)
{
	t_ast_node		*ast_node;
	t_ast_node		*ast_root_node;
	t_ddeque		*stack;
	t_ast_node		*top;
	t_ast_node		**children;

	stack = ddeque_init();
	ddeque_push_value_top(stack, production_to_child((t_production){TOKEN, {&(t_token){TOK_EOL, NULL}}}));
	ddeque_push_value_top(stack, production_to_child((t_production){COMPLETE_COMMAND, {0}}));
	ast_node = new_ast_nonterm(COMPLETE_COMMAND, NULL);
	ast_root_node = ast_node;
	while (1)
	{
		top = stack->head->data;
		free(ddeque_pop_top(stack));
		if (top->type != TOKEN)
		{
			children = productions_to_children(get_production(top->type, tokens->head->data));
			print_children(children);
			push_productions(ast_root_node, stack, children);
			ast_node->data.children = children;
			ast_node = ast_node->data.children[0];
		}
		else
		{
			if (top->data.token->type == TOK_EPSILON)
			{
				ast_node = stack->head->data;
				continue ;
			}
			else if (get_token_type(tokens) == TOK_EOL && top->data.token->type == TOK_EOL)
				break ;
			else if (get_token_type(tokens) == top->data.token->type)
			{
				ast_node->data.token->str = get_token_str(tokens);
				ast_node = stack->head->data;
				ddeque_rotate(tokens, 1);
			}
			else if (get_token_type(tokens) == TOK_EOL)
				internal_error("build_ast: Stack not empty", __LINE__);
			else
				internal_error("build_ast: Stacks don't match", __LINE__);
		}
	}
	ddeque_free(stack, ft_free);
	/* ast_node = return_example_ast(); */
	return (ast_root_node);
}

