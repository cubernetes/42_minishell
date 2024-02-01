/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/01 07:30:20 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h> /* needed for FILE typedef needed by readline */
#include <readline/history.h> /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h> /* free() */
#include <unistd.h> /* STDERR_FILENO */

const char	*token_type_to_string(t_token_type type)
{
	if (type == TOK_OR)
		return (STR_TOK_OR);
	else if (type == TOK_OVERRIDE)
		return (STR_TOK_OVERRIDE);
	else if (type == TOK_INPUT)
		return (STR_TOK_INPUT);
	else if (type == TOK_APPEND)
		return (STR_TOK_APPEND);
	else if (type == TOK_HEREDOC)
		return (STR_TOK_HEREDOC);
	else if (type == TOK_PIPE)
		return (STR_TOK_PIPE);
	else if (type == TOK_AND)
		return (STR_TOK_AND);
	else if (type == TOK_OR)
		return (STR_TOK_OR);
	else if (type == TOK_L_PAREN)
		return (STR_TOK_L_PAREN);
	else if (type == TOK_R_PAREN)
		return (STR_TOK_R_PAREN);
	else if (type == TOK_SQUOTE_STR)
		return (STR_TOK_SQUOTE_STR);
	else if (type == TOK_DQUOTE_STR)
		return (STR_TOK_DQUOTE_STR);
	else if (type == TOK_WORD)
		return (STR_TOK_WORD);
	else if (type == TOK_EOL)
		return (STR_TOK_EOL);
	else if (type == TOK_EPSILON)
		return (STR_TOK_EPSILON);
	else if (type == TOK_ERROR)
		return (STR_TOK_ERROR);
	return (STR_TOK_UNKNOWN);
}

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

void	repeat_string(const char *str, size_t n)
{
	while (n--)
		ft_printf("%s", str);
}

/* TODO: should not be used anywhere for submission!!! */
void	internal_error(const char *err, int line_no)
{
	ft_dprintf(STDERR_FILENO, "\033[31mLine %d: %s\033[m\n", line_no, err);
	exit(42);
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

	DEBUG(2, "%s", "ENTRY AST PRINT");
	if (!ast_node)
		internal_error("ast_print_with_depth: ast_node is NULL", __LINE__);
	repeat_string("    ", n);
	DEBUG(2, "%s", "BEFORE CHECK");
	if (ast_node->type != TOKEN)
	{
		DEBUG(2, "%s", "NONTERMINAL");
		ft_printf("- <%s>\n", ast_node_type_to_string(ast_node->type));
		DEBUG(2, "getting children:%s", "");
		DEBUG(2, "children: %p", ast_node->data.children);
		DEBUG(2, "%s", "ok");
		if (ast_node->data.children)
		{
			DEBUG(2, "%s", "path 1");
			DEBUG(2, "children[0]: %p", ast_node->data.children[0]);
			DEBUG(2, "children[0].type: %s", ast_node_type_to_string(ast_node->data.children[0]->type));
		}
		tmp_children = ast_node->data.children;
		DEBUG(2, "tmp_children: %p", tmp_children);
		while (tmp_children && *tmp_children && !ast_node_is_null(*tmp_children))
		{
			DEBUG(2, "%s", "path 2");
			ast_print_with_depth(*tmp_children, n + 1);
			++tmp_children;
		}
		DEBUG(2, "after", tmp_children);
	}
	else
	{
		DEBUG(2, "%s", "TOKEN");
		ft_printf("- %s (\033[31m%s\033[m)\n",
			token_type_to_string(ast_node->data.token->type),
			ast_node->data.token->str);
	}
}

void	ast_print(t_ast_node *ast_node)
{
	ast_print_with_depth(ast_node, 0);
}

t_bool	free_token(void *data)
{
	t_token	*token;

	if (!data)
		return (TRUE);
	token = (t_token *)data;
	(free(token->str), token->str = NULL);
	(free(token), token = NULL);
	return (TRUE);
}

char	*get_token_str(t_ddeque *tokens)
{
	return (((t_token *)tokens->head->data)->str);
}

t_token_type	get_token_type(t_ddeque *tokens)
{
	return (((t_token *)tokens->head->data)->type);
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
		DEBUG(2, "production is NULL %s", "");
		return ;
	}
	if (production->type == TOKEN)
		ft_printf("%s\033[32m%s\033[m (\033[31m%s\033[m)", second,
			token_type_to_string(production->data.token->type),
			production->data.token->str);
	else
		ft_printf("%s\033[32m%s\033[m", second, ast_node_type_to_string(production->type));
}

void	push_productions(t_ast_node *ast_node, t_ddeque *stack, t_production **productions)
{
	DEBUG(1, "%s", "STACK0");
	ddeque_print(stack, print_production);
	DEBUG(1, "%s", "AST0");
	ast_print(ast_node);
	if (ast_node_is_null(*productions))
		return ;
	DEBUG(1, "%s", "STACK1");
	ddeque_print(stack, print_production);
	DEBUG(1, "%s", "AST1");
	ast_print(ast_node);
	push_productions(ast_node, stack, productions + 1);
	DEBUG(1, "%s", "STACK2");
	ddeque_print(stack, print_production);
	DEBUG(1, "%s", "AST2");
	ast_print(ast_node);
	ddeque_push_value_top(stack, *productions);
	DEBUG(2, "productions: %p", productions);
	DEBUG(2, "*productions: %p", *productions);
	DEBUG(2, "productions[0]->data: %p", productions[0]->data);
	DEBUG(1, "%s", "STACK3");
	ddeque_print(stack, print_production);
	DEBUG(1, "%s", "AST3");
	ast_print(ast_node);
	DEBUG(1, "%s", "DONE PUSHING PRODUCTIONS");
}

/* TODO: NOT REQUIRED: add basic prompt expansion */
char	*expand_prompt(const char *prompt_string)
{
	return (ft_strdup(prompt_string));
}

void	update_state(t_state *state)
{
	state->ps0 = expand_prompt(PS0);
	state->ps1 = expand_prompt(PS1);
}

void	skip_whitespace(const char **line)
{
	while (ft_isspace(**line))
		++(*line);
}

/* TODO: change return type to t_token later */
void	*new_token(char *str, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (!token)
		internal_error("new_token: malloc", __LINE__);
	token->str = str;
	token->type = type;
	return (token);
}

t_bool	push_token(const char **line, t_ddeque *tokens, size_t token_len,
	t_token_type type)
{
	char	*str;

	str = ft_strndup(*line, token_len);
	if (!str)
		internal_error("push_token: ft_strndup", __LINE__);
	ddeque_push_value_bottom(tokens, new_token(str, type));
	*line += token_len;
	return (TRUE);
}

t_bool	tokenize_fixed_len_tokens(const char **line, t_ddeque *tokens)
{
	t_bool	pushed;

	pushed = FALSE;
	if (**line == '\0')
		pushed = push_token(line, tokens, 1, TOK_EOL);
	else if (**line == '>' && *(*line + 1) == '>')
		pushed = push_token(line, tokens, 2, TOK_APPEND);
	else if (**line == '<' && *(*line + 1) == '<')
		pushed = push_token(line, tokens, 2, TOK_HEREDOC);
	else if (**line == '&' && *(*line + 1) == '&')
		pushed = push_token(line, tokens, 2, TOK_AND);
	else if (**line == '|' && *(*line + 1) == '|')
		pushed = push_token(line, tokens, 2, TOK_OR);
	else if (**line == '>')
		pushed = push_token(line, tokens, 1, TOK_OVERRIDE);
	else if (**line == '<')
		pushed = push_token(line, tokens, 1, TOK_INPUT);
	else if (**line == '|')
		pushed = push_token(line, tokens, 1, TOK_PIPE);
	else if (**line == '(')
		pushed = push_token(line, tokens, 1, TOK_L_PAREN);
	else if (**line == ')')
		pushed = push_token(line, tokens, 1, TOK_R_PAREN);
	return (pushed);
}

t_bool	tokenize_single_quoted_string(const char **line, t_ddeque *tokens)
{
	size_t		len;
	const char	*tmp;

	if (**line == '\'')
	{
		len = 0;
		tmp = *line + 1;
		while (*tmp != '\'' && *tmp != '\0')
		{
			++tmp;
			++len;
		}
		if (*tmp == '\'')
		{
			++(*line);
			push_token(line, tokens, len, TOK_SQUOTE_STR);
			++(*line);
		}
		else
			push_token(line, tokens, len + 1, TOK_ERROR);
		return (TRUE);
	}
	return (FALSE);
}

t_bool	tokenize_double_quoted_string(const char **line, t_ddeque *tokens)
{
	size_t		len;
	const char	*tmp;

	if (**line == '"')
	{
		len = 0;
		tmp = *line + 1;
		while (*tmp != '"' && *tmp != '\0')
		{
			++tmp;
			++len;
		}
		if (*tmp == '"')
		{
			++(*line);
			push_token(line, tokens, len, TOK_DQUOTE_STR);
			++(*line);
		}
		else
			push_token(line, tokens, len + 1, TOK_ERROR);
		return (TRUE);
	}
	return (FALSE);
}

t_bool	is_word_char(char c)
{
	return ((t_bool)(
		ft_isprint(c)
		&& !ft_isspace(c)
		&& !ft_strchr("><()'\"|", c)
	));
}

t_bool	is_not_and_and(const char *line)
{
	if (line[0] == '&' && line[1] == '&')
		return (FALSE);
	return (TRUE);
}

t_bool	tokenize_word(const char **line, t_ddeque *tokens)
{
	size_t		len;
	const char	*tmp;

	if (is_word_char(**line) && is_not_and_and(*line))
	{
		len = 0;
		tmp = *line;
		while (is_word_char(*tmp) && is_not_and_and(tmp))
		{
			++tmp;
			++len;
		}
		push_token(line, tokens, len, TOK_WORD);
		return (TRUE);
	}
	return (FALSE);
}

void	tokenize_variable_len_tokens(const char **line, t_ddeque *tokens)
{
	t_bool	pushed;

	pushed = FALSE;
	pushed += tokenize_single_quoted_string(line, tokens);
	pushed += tokenize_double_quoted_string(line, tokens);
	pushed += tokenize_word(line, tokens);
	if (!pushed)
		internal_error("tokenize_variable_len_tokens: lexing", __LINE__);
}

/* environment variable expansion is NOT happening in the tokenization phase*/
/* wildcard expansion (globbing) is also not happening here*/
/* those are responsibilities while or after building the AST */
t_ddeque	*tokenize(const char *line)
{
	t_ddeque		*tokens;

	tokens = ddeque_init();
	while (TRUE)
	{
		skip_whitespace(&line);
		if (!tokenize_fixed_len_tokens(&line, tokens))
			tokenize_variable_len_tokens(&line, tokens);
		if (((t_token *)tokens->head->prev->data)->type == TOK_EOL)
			break ;
	}
	return (tokens);
}

t_ast_node	*new_ast_node(t_ast_node_type type, t_ast_node_data data)
{
	t_ast_node	*ast_node;

	ast_node = malloc(sizeof(*ast_node));
	if (!ast_node)
		internal_error("new_ast_node: malloc", __LINE__);
	ast_node->type = type;
	ast_node->data = data;
	return (ast_node);
}

t_ast_node	*new_ast_token(t_token_type type, char *str)
{
	t_ast_node	*ast_node;

	ast_node = malloc(sizeof(*ast_node));
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

	ast_node = malloc(sizeof(*ast_node));
	ast_node->type = type;
	ast_node->data.children = children;
	return (ast_node);
}

t_ast_node	**new_children(t_ast_node **children)
{
	t_ast_node	**new_children;
	size_t		i;

	i = 0;
	while (children[i])
		++i;
	new_children = malloc(sizeof(*new_children) * (++i));
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

t_bool	dont_free(void *data)
{
	(void)data;
	return (1);
}

void	print_token(void *data, t_bool first)
{
	t_token	*token;

	token = (t_token *)data;
	if (first)
		ft_printf("<\033[31m%s\033[m> (%s)", token->str,
			token_type_to_string(token->type));
	else
		ft_printf(" -> <\033[31m%s\033[m> (%s)", token->str,
			token_type_to_string(token->type));
}

t_ast_node	*production_to_child(t_production prodcution)
{
	t_ast_node	*child;

	child = malloc(sizeof(*child));
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
	{
		DEBUG(0, "production[%d]: %s", size, ast_node_type_to_string(productions[size].type));
		++size;
	}
	DEBUG(0, "size is %d", size);
	children = malloc(sizeof(*children) * (size_t)(size + 1));
	size = 0;
	while (!ast_node_is_null(productions + size))
	{
		children[size] = production_to_child(productions[size]);
		++size;
	}
	DEBUG(0, "size end is %d", size);
	children[size] = NULL;
	return (children);
}

t_bool	ft_free(void *ptr)
{
	free(ptr);
	return (TRUE);
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
		DEBUG(0, "Ast before 0 :", "");
		ast_print(ast_root_node);
		top = stack->head->data;
		DEBUG(0, "Ast before 1 :", "");
		ast_print(ast_root_node);
		DEBUG(0, "printing stack %s", "");
		ddeque_print(stack, print_production);
		DEBUG(0, "Ast before 2 :", "");
		ast_print(ast_root_node);
		ddeque_print(tokens, print_token);
		DEBUG(0, "Ast before 3 :", "");
		ast_print(ast_root_node);
		ast_print(ast_root_node);
		DEBUG(0, "Ast before 4 :", "");
		ast_print(ast_root_node);
		ast_print(ast_node);
		DEBUG(0, "Ast before 5 :", "");
		ast_print(ast_root_node);
		ft_printf("\n");
		DEBUG(0, "Ast before 6 :", "");
		ast_print(ast_root_node);
		free(ddeque_pop_top(stack));
		DEBUG(0, "Ast before 7 :", "");
		ast_print(ast_root_node);
		if (top->type != TOKEN)
		{
			DEBUG(0, "Ast before 8 :", "");
			ast_print(ast_root_node);
			children = productions_to_children(get_production(top->type, tokens->head->data));
			DEBUG(0, "Ast before 9 :", "");
			ast_print(ast_root_node);
			DEBUG(0, "Pushing these children:", "");
			print_children(children);
			DEBUG(0, "Stack again:%s", "");
			ddeque_print(stack, print_production);
			DEBUG(0, "Ast before 10 :", "");
			ast_print(ast_root_node);
			DEBUG(0, "Ast before 11 :", "");
			ast_print(ast_root_node);
			DEBUG(0, "Ast before 12 :", "");
			ast_print(ast_root_node);
			push_productions(ast_root_node, stack, children);
			DEBUG(0, "Ast before:", "");
			ast_print(ast_root_node);
			ast_node->data.children = children;
			DEBUG(0, "Ast middle", "");
			ast_print(ast_root_node);
			ast_node = ast_node->data.children[0];
			DEBUG(0, "Ast after", "");
			ast_print(ast_root_node);
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

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	(void)ast_node;
}

t_bool	free_datastructures(char **line, t_ddeque **tokens,
	t_ast_node **ast_node)
{
	(free(*line), *line = NULL);
	((void)ddeque_free(*tokens, free_token), *tokens = NULL);
	((void)ast_free(*ast_node), *ast_node = NULL);
	return (TRUE);
}

t_bool	free_state(t_state *state, char **line, t_ddeque **tokens,
	t_ast_node **ast_node)
{
	(void)free_datastructures(line, tokens, ast_node);
	(free(state->ps0), state->ps0 = NULL);
	(free(state->ps1), state->ps1 = NULL);
	return (TRUE);
}

void	setup_signals(void)
{
}

/* TODO: what if readline returns NULL? */
/* TODO: use/think about rl_end (and other rl vars) */
/* TODO: remove DEBUG macros */
/* TODO: remove ddeque_print(tokens, print_token); */
/* TODO: glob expansion, env var expansion, and quote handling */
/* TOOD: parsing */
/* TODO: execution */
/* TODO: signal handling */
/* TODO: protect all mallocs (e.g. xmalloc(size_t n, int lineno), etc.) */
/* TODO: check for NULL when using any function that returns a malloced ptr */
/* TODO: make functions static where needed */
int	main(void)
{
	static t_state	state;
	static int		i = 10;
	char			*line;
	t_ddeque		*tokens;
	t_ast_node		*ast_root_node;

	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();
	while (i--)
	{
		update_state(&state);
		line = readline(state.ps1);
		if (!line && free_state(&state, &line, &tokens, &ast_root_node))
			break ;
		tokens = tokenize(line);
		ddeque_print(tokens, print_token);
		ft_printf("\n");
		ast_root_node = build_ast(tokens);
		ast_print(ast_root_node);
		execute(ast_root_node);
		(void)free_state(&state, &line, &tokens, &ast_root_node);
	}
	return (0);
}
