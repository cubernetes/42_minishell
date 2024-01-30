/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/30 18:59:30 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h> /* needed for FILE typedef needed by readline */
#include <readline/history.h> /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h> /* free() */
#include <unistd.h> /* STDERR_FILENO */

typedef t_ast_node	t_production;

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

void   push_productions(t_ddeque *stack, t_production *productions)
{
	if (productions->type == 0 && productions->data.token == NULL
		&& productions->data.children == NULL)
		return ;
	push_productions(stack, productions + 1);
	ddeque_push_value_top(stack, productions);
}

void   print_production(void *data, t_bool first)
{
	t_production	*production;
	const char		*second;

	production = (t_production *)data;
	if (first)
		second = "";
	else
		production = (t_production *)data;
	if (first)
		second = "";
	else
		second = ", ";
	if (production->type == TOKEN)
		ft_printf("%s\033[32m%s\033[m (\033[31m%s\033[m)", second,
			token_type_to_string(production->data.token->type),
			production->data.token->str);
	else
		ft_printf("%s\033[32m%s\033[m", second, ast_node_type_to_string(production->type));
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

/* TODO: should not be used anywhere for submission!!! */
void	internal_error(const char *err, int line_no)
{
	ft_dprintf(STDERR_FILENO, "\033[31mLine %d: %s\033[m\n", line_no, err);
	exit(42);
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
int	get_production_idx(t_ast_node_type nonterm, t_token_type token)
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

	return (transition_table[nonterm][token]);
}

/* TODO: Refactor */
/* TODO: Don't use 0 as NULL */
t_production   *get_production(t_ast_node_type nonterm, t_token_type token)
{
	static t_token		tokens[] = {{TOK_EOL, 0}, {TOK_AND, 0}, {TOK_OR, 0}, \
		{TOK_PIPE, 0}, {TOK_L_PAREN, 0}, {TOK_R_PAREN, 0}, {TOK_WORD, 0}, \
		{TOK_OVERRIDE, 0}, {TOK_APPEND, 0}, {TOK_INPUT, 0}, {TOK_HEREDOC, 0}, \
		{TOK_EPSILON, 0}, {TOK_SQUOTE_STR, 0}, {TOK_DQUOTE_STR, 0}, \
		{TOK_ERROR, 0}};
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

t_ast_node     *build_ast(t_ddeque *tokens)
{
	t_ast_node		*ast_node;
	t_ddeque		*stack;
	t_ast_node		*top;
	t_production	*productions;

	stack = ddeque_init();
	ddeque_push_value_top(stack, &(t_production){TOKEN, {&(t_token){TOK_EOL, NULL}}});
	ddeque_push_value_top(stack, &(t_production){COMPLETE_COMMAND, {0}});
	while (1)
	{
		top = stack->head->data;
		ddeque_print(stack, print_production);
		free(ddeque_pop_top(stack));
		if (get_token_type(tokens) == TOK_EOL)
		{
			if (top->type == TOKEN && top->data.token->type == TOK_EOL)
				break ;
			internal_error("build_ast: Stack not empty", __LINE__);
		}
		else if (top->type == TOKEN && get_token_type(tokens) == top->data.token->type)
			tokens->head = tokens->head->next;
		else if (top->type != TOKEN)
		{
			productions = get_production(top->type, get_token_type(tokens));
			push_productions(stack, productions);
		}
		else
			internal_error("build_ast: Stacks don't match", __LINE__);
	}
	ddeque_free(stack, dont_free);
	ast_node = return_example_ast();
	return (ast_node);
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

void	repeat_string(const char *str, size_t n)
{
	while (n--)
		ft_printf("%s", str);
}

void	ast_print_with_depth(t_ast_node *ast_node, size_t n)
{
	t_ast_node	**tmp_children;

	repeat_string("    ", n);
	if (ast_node->type != TOKEN)
	{
		ft_printf("- <%s>\n", ast_node_type_to_string(ast_node->type));
		tmp_children = ast_node->data.children;
		while (*tmp_children)
		{
			ast_print_with_depth(*tmp_children, n + 1);
			++tmp_children;
		}
	}
	else
		ft_printf("- %s (\033[31m%s\033[m)\n",
			token_type_to_string(ast_node->data.token->type),
			ast_node->data.token->str);
}

void	ast_print(t_ast_node *ast_node)
{
	ast_print_with_depth(ast_node, 0);
}

void	setup_signals(void)
{
	int	i;
	
	i = 0;
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
