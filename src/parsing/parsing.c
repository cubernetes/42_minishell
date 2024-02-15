/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 07:40:13 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/15 13:31:56 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../libft/libft.h"
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

t_bool	ast_node_is_null(t_ast_node *ast_node)
{
	if (!ast_node)
		return (TRUE);
	if (ast_node->type == 0)
		if (ast_node->data.token == NULL && ast_node->data.children == NULL)
			return (TRUE);
	return (FALSE);
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

void	repeat_string(const char *str, int n, t_bool color)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		if (color)
		{
			ft_putstr("\033[");
			ft_putnbr((i % 7) + 31);
			ft_putstr("m");
		}
		ft_printf("%s", str);
		if (color)
			ft_putstr("\033[m");
	}
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
	ast_node->data.token = new_token(str, type, TRUE);
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

void	ast_print_with_depth(t_ast_node *ast_node, int n)
{
	t_ast_node	**tmp_children;

	repeat_string("|   ", n, TRUE);
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

t_ast_node	*return_example_parse_tree(void)
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

t_ast_node	*return_example_ast(void)
{
	return (\
	new_ast_nonterm(COMPLETE_COMMAND, new_children((t_children){\
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_HEREDOC, "<<"), \
					new_ast_m_token(TOK_WORD, "heredoc"), \
				NULL})), \
				new_ast_m_token(TOK_WORD, "echo"), \
				new_ast_m_token(TOK_WORD, "hi"), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_INPUT, "<"), \
					new_ast_m_token(TOK_WORD, "infile"), \
				NULL})), \
				new_ast_m_token(TOK_WORD, "grep"), \
				new_ast_m_token(TOK_WORD, "^h"), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_OVERRIDE, ">"), \
					new_ast_m_token(TOK_WORD, "somefile"), \
				NULL})), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_OVERRIDE, ">"), \
					new_ast_m_token(TOK_WORD, "anotherfile"), \
				NULL})), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_m_token(TOK_WORD, "nl"), \
			NULL})), \
		NULL})), \
		new_ast_m_token(TOK_AND, "&&"), \
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
			new_ast_nonterm(COMPLETE_COMMAND, new_children((t_children){\
				new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
					new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_HEREDOC, "<<"), \
							new_ast_m_token(TOK_WORD, "heredoc"), \
						NULL})), \
						new_ast_m_token(TOK_WORD, "echo"), \
						new_ast_m_token(TOK_WORD, "hi"), \
					NULL})), \
					new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_INPUT, "<"), \
							new_ast_m_token(TOK_WORD, "infile"), \
						NULL})), \
						new_ast_m_token(TOK_WORD, "grep"), \
						new_ast_m_token(TOK_WORD, "^h"), \
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_OVERRIDE, ">"), \
							new_ast_m_token(TOK_WORD, "somefile"), \
						NULL})), \
						new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
							new_ast_m_token(TOK_OVERRIDE, ">"), \
							new_ast_m_token(TOK_WORD, "anotherfile"), \
						NULL})), \
					NULL})), \
					new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
						new_ast_m_token(TOK_WORD, "nl"), \
					NULL})), \
				NULL})), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_m_token(TOK_WORD, "grep"), \
				new_ast_m_token(TOK_WORD, "h"), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_APPEND, ">>"), \
					new_ast_m_token(TOK_WORD, "appendfile.txt"), \
				NULL})), \
			NULL})), \
		NULL})), \
		new_ast_m_token(TOK_AND, "||"), \
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){\
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_INPUT, "<"), \
					new_ast_m_token(TOK_WORD, "infile"), \
				NULL})), \
				new_ast_m_token(TOK_WORD, "hello world"), \
				new_ast_nonterm(IO_REDIRECT, new_children((t_children){\
					new_ast_m_token(TOK_OVERRIDE, ">"), \
					new_ast_m_token(TOK_WORD, "/dev/null"), \
				NULL})), \
			NULL})), \
			new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){\
				new_ast_m_token(TOK_WORD, "grep"), \
				new_ast_m_token(TOK_DQUOTE_STR, "^hello world"), \
			NULL})), \
		NULL})), \
	NULL}))
	);
}

/* return the index of the production to use */
/* TODO: fix error handling */
int	get_production_idx(t_ast_node_type nonterm, t_token *token)
{
	static int	transition_table[NUM_NONTERMS][NUM_TOKENS] = {
	{19, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0},
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
		if (!*token->str)
			minishell_error(2, "syntax error near unexpected token `newline'");
		minishell_error(2, "syntax error near unexpected token `%s'",
			token->str);
	}
	return (production_idx);
}

/* TODO: Refactor */
/* TODO: Don't use 0 as NULL */
t_production	*get_production(t_ast_node_type nonterm, t_token *token)
{
	static t_token		tks[] = {{0}, {TOK_EOL, "", TRUE}, {TOK_AND, "", TRUE}, \
		{TOK_OR, "", TRUE}, {TOK_PIPE, "", TRUE}, {TOK_L_PAREN, "", TRUE}, {TOK_R_PAREN, "", TRUE}, \
		{TOK_WORD, "", TRUE}, {TOK_OVERRIDE, "", TRUE}, {TOK_APPEND, "", TRUE}, {TOK_INPUT, "", TRUE}, \
		{TOK_HEREDOC, "", TRUE}, {TOK_EPSILON, "", TRUE}, {TOK_SQUOTE_STR, "", TRUE}, \
		{TOK_DQUOTE_STR, "", TRUE}, {TOK_ERROR, "", TRUE}};
	static t_production	productions[][4] = {\
		{{PIPE_SEQUENCE, {0}}, {COMPLETE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tks[TOK_EPSILON]}}, {0}}, \
		{{AND_OR, {0}}, {PIPE_SEQUENCE, {0}}, {COMPLETE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tks[TOK_AND]}}, {0}}, {{TOKEN, {&tks[TOK_OR]}}, {0}}, \
		{{COMMAND, {0}}, {PIPE_SEQUENCE_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tks[TOK_EPSILON]}}, {0}}, \
		{{TOKEN, {&tks[TOK_PIPE]}}, {COMMAND, {0}}, {PIPE_SEQUENCE_TAIL, {0}}, {0}}, \
		{{SIMPLE_COMMAND, {0}}, {0}}, {{COMPOUND_COMMAND, {0}}, {0}}, \
		{{TOKEN, {&tks[TOK_L_PAREN]}}, {COMPLETE_COMMAND, {0}}, {TOKEN, {&tks[TOK_R_PAREN]}}, {0}}, \
		{{IO_REDIRECT, {0}}, {TOKEN, {&tks[TOK_WORD]}}, {SIMPLE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tks[TOK_EPSILON]}}, {0}}, \
		{{IO_REDIRECT, {0}}, {TOKEN, {&tks[TOK_WORD]}}, {SIMPLE_COMMAND_TAIL, {0}}, {0}}, \
		{{TOKEN, {&tks[TOK_EPSILON]}}, {0}}, \
		{{TOKEN, {&tks[TOK_APPEND]}}, {0}}, \
		{{TOKEN, {&tks[TOK_HEREDOC]}}, {0}}, \
		{{TOKEN, {&tks[TOK_OVERRIDE]}}, {0}}, \
		{{TOKEN, {&tks[TOK_INPUT]}}, {0}}, {{TOKEN, {&tks[TOK_EPSILON]}}, {0}}};

	return (productions[get_production_idx(nonterm, token)]);
}

t_ast_node	*production_to_child(t_production prodcution)
{
	t_ast_node	*child;

	child = ft_malloc(sizeof(*child));
	ft_memcpy(child, &prodcution, sizeof(prodcution));
	if (prodcution.type == TOKEN)
		child->data.token = new_token(child->data.token->str,
			child->data.token->type, TRUE);
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
t_ast_node	*build_parse_tree(t_ddeque *tokens)
{
	t_ast_node		*ast_node;
	t_ast_node		*ast_root_node;
	t_ddeque		*stack;
	t_ast_node		*top;
	t_ast_node		**children;

	stack = ddeque_init();
	ddeque_push_value_top(stack, production_to_child((t_production){TOKEN, {&(t_token){TOK_EOL, "", TRUE}}}));
	ddeque_push_value_top(stack, production_to_child((t_production){COMPLETE_COMMAND, {0}}));
	ast_node = new_ast_nonterm(COMPLETE_COMMAND, NULL);
	ast_root_node = ast_node;
	while (1)
	{
		top = stack->head->data;
		ddeque_pop_top(stack);
		if (top->type != TOKEN)
		{
			children = productions_to_children(get_production(top->type, tokens->head->data));
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
				minishell_error(2, "syntax error near unexpected token `%s'",
					get_token_str_nl(tokens));
			else
				minishell_error(2, "syntax error near unexpected token `%s'",
					get_token_str_nl(tokens));
		}
	}
	return (ast_root_node);
}

t_ast_node	*build_ast(t_ddeque *tokens)
{
	t_ast_node	*parse_tree;
	t_ast_node	*ast;

	parse_tree = build_parse_tree(tokens);
	ast = return_example_ast();
	return (ast);
}
