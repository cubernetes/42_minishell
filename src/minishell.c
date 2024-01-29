/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/29 03:34:09 by tosuman          ###   ########.fr       */
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
	else if (type == TOK_ERROR)
		return (STR_TOK_ERROR);
	return (STR_TOK_UNKNOWN);
}

const char	*ast_node_type_to_string(t_ast_node_type type)
{
	if (type == TOKEN)
		return (STR_TOKEN);
	else if (type == SIMPLE_COMMAND)
		return (STR_SIMPLE_COMMAND);
	else if (type == COMPLETE_COMMAND)
		return (STR_COMPLETE_COMMAND);
	else if (type == PIPE_SEQUENCE)
		return (STR_PIPE_SEQUENCE);
	else if (type == COMMAND)
		return (STR_COMMAND);
	else if (type == COMPOUND_COMMAND)
		return (STR_COMPOUND_COMMAND);
	else if (type == SIMPLE_COMMAND)
		return (STR_SIMPLE_COMMAND);
	else if (type == IO_REDIRECT)
		return (STR_IO_REDIRECT);
	else
		return (STR_AST_NODE_TYPE_UNKNOWN);
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
	new_ast_nonterm(COMPLETE_COMMAND, new_children((t_children){
		new_ast_nonterm(PIPE_SEQUENCE, new_children((t_children){
			new_ast_nonterm(COMMAND, new_children((t_children){
				new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){
					new_ast_m_token(TOK_WORD, "echo"),
					new_ast_m_token(TOK_WORD, "hi"),
				NULL})),
			NULL})),
			new_ast_m_token(TOK_PIPE, "|"),
			new_ast_nonterm(COMMAND, new_children((t_children){
				new_ast_nonterm(SIMPLE_COMMAND, new_children((t_children){
					new_ast_m_token(TOK_WORD, "grep"),
					new_ast_m_token(TOK_WORD, "^h"),
				NULL})),
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

void	cleanup(void)
{
}

char	*get_token_str(t_ddeque *tokens)
{
	return (((t_token *)tokens->head->data)->str);
}

t_token_type	get_token_type(t_ddeque *tokens)
{
	return (((t_token *)tokens->head->data)->type);
}

void	throw_syntax_error(t_ddeque *tokens)
{
	if (get_token_type(tokens) != TOK_EOL)
		ft_printf("minishell: syntax error near unexpected token `%s'\n",
			get_token_str(tokens));
	else
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
	cleanup();
	exit(42);
}

t_bool	accept(t_ddeque *tokens, t_token_type type)
{
	if (get_token_type(tokens) == type)
	{
		tokens->head = tokens->head->next;
		return (TRUE);
	}
	return (FALSE);
}

t_bool	expect(t_ddeque *tokens, t_token_type type)
{
	if (accept(tokens, type))
		return (TRUE);
	throw_syntax_error(tokens);
	return (FALSE);
}

t_ast_node	*parse_complete_command(t_ddeque *tokens)
{
	t_ast_node	*ast_node;

	(void)tokens;
	ast_node = malloc(sizeof(*ast_node));
	return (ast_node);
}

/* change return type to t_ast_node* */
void	parse_io_redirect(t_ddeque *tokens)
{
	if (accept(tokens, TOK_OVERRIDE) || accept(tokens, TOK_APPEND)
		|| accept(tokens, TOK_INPUT) || accept(tokens, TOK_HEREDOC))
		expect(tokens, TOK_WORD);
	else
		throw_syntax_error(tokens);
}

void	parse_simple_command(t_ddeque *tokens)
{
	if (!accept(tokens, TOK_WORD))
		parse_io_redirect(tokens);
	while (TRUE)
	{
	}
}

t_bool	fully_consumed(t_ddeque *tokens)
{
	if (get_token_type(tokens) == TOK_EOL)
		return (TRUE);
	return (FALSE);
}

/* return (return_example_ast()); */
t_ast_node	*parse(t_ddeque *tokens)
{
	t_ast_node	*ast_node;

	ast_node = parse_complete_command(tokens);
	/* ast_node = parse_io_redirect(tokens); */
	if (!fully_consumed(tokens))
		throw_syntax_error(tokens);
	return (ast_node);
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	(void)ast_node;
}

t_bool	dont_free(void *data)
{
	(void)data;
	return (1);
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

/* TODO: pgrussin */
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
int	actual_main(void)
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
		ast_root_node = parse(tokens);
		ast_print(ast_root_node);
		execute(ast_root_node);
		(void)free_state(&state, &line, &tokens, &ast_root_node);
	}
	return (0);
}

int	test_main(void)
{
	char		*line;
	t_ddeque	*tokens;

	tokens = NULL;
	line = readline("$ ");
	if (line)
	{
		tokens = tokenize(line);
		ddeque_print(tokens, print_token);
		parse_simple_command(tokens);
		free(line);
		ddeque_free(tokens, free_token);
	}
	return (0);
}

int	main(void)
{
	int	ret;

	/* ret = actual_main(); */
	ret = test_main();
	return (ret);
}
