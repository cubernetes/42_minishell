/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/28 02:12:44 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h> /* needed for FILE typedef needed by readline */
#include <readline/history.h> /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h> /* free() */
#include <unistd.h> /* STDERR_FILENO */

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

int	push_token(const char **line, t_ddeque *tokens, size_t token_len,
	t_token_type token_type)
{
	char	*str;

	str = ft_strndup(*line, token_len);
	if (!str)
		internal_error("push_token: ft_strndup", __LINE__);
	ddeque_push_value_bottom(tokens, new_token(str, token_type));
	*line += token_len;
	return (1);
}

int	tokenize_fixed_len_tokens(const char **line, t_ddeque *tokens)
{
	int	pushed;

	pushed = 0;
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

int	tokenize_single_quoted_string(const char **line, t_ddeque *tokens)
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
		return (1);
	}
	return (0);
}

int	tokenize_double_quoted_string(const char **line, t_ddeque *tokens)
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
		return (1);
	}
	return (0);
}

int	is_word_char(char c)
{
	return (
		ft_isprint(c)
		&& !ft_isspace(c)
		&& !ft_strchr("><()'\"|", c)
	);
}

int	is_not_and_and(const char *line)
{
	if (line[0] == '&' && line[1] == '&')
		return (0);
	return (1);
}

int	tokenize_word(const char **line, t_ddeque *tokens)
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
		return (1);
	}
	return (0);
}

void	tokenize_variable_len_tokens(const char **line, t_ddeque *tokens)
{
	int	pushed;

	pushed = 0;
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
	while (1)
	{
		skip_whitespace(&line);
		if (!tokenize_fixed_len_tokens(&line, tokens))
			tokenize_variable_len_tokens(&line, tokens);
		if (((t_token *)tokens->head->prev->data)->type == TOK_EOL)
			break ;
	}
	return (tokens);
}

t_ast	*new_ast_node(t_token *token, t_ast **children)
{
	t_ast	*ast;

	ast = malloc(sizeof(*ast));
	if (!ast)
		internal_error("new_ast_node: malloc", __LINE__);
	ast->token = token;
	ast->children = children;
	return (ast);
}

t_ast	*parse(t_ddeque *tokens)
{
	t_ast	**children;

	children[0] = new_ast_node(new_token(NULL, SIMPLE_COMMAND), NULL);
	children[1] = new_ast_node(tokens->head->data, NULL);
	children[2] = NULL;
	return (new_ast_node(new_token(NULL, SIMPLE_COMMAND), children));
}

void	execute(t_ast *ast)
{
	(void)ast;
}

void	dont_free(void *data)
{
	(void)data;
}

void	free_token(void *data)
{
	t_token	*token;

	if (!data)
		return ;
	token = (t_token *)data;
	DEBUG(0, "token->str: %s", token->str);
	DEBUG(0, "%s", "freeing token->str");
	(free(token->str), token->str = NULL);
	DEBUG(0, "%s", "freeing token");
	(free(token), token = NULL);
	DEBUG(0, "%s", "done");
}

/* ast_free DOES NOT free it's tokens, since the tokens are bookkept through
 * the linear token datastructure (t_ddeque *tokens), which does the freeing
 * through the function ddeque_free (which in turn takes the free_token
 * 'destructor' function).
 */
/* (free_token(ast->token), ast->token = NULL); */ /* <- don't add this */
void	ast_free(t_ast *ast)
{
	t_ast	**orig_children;

	if (!ast)
		return ;
	orig_children = ast->children;
	while (ast->children)
	{
		ast_free(*ast->children);
		++ast->children;
	}
	(free(orig_children), ast->children = NULL);
}

void	free_datastructures(char **line, t_ddeque **tokens, t_ast **ast)
{
	(free(*line), *line = NULL);
	(ddeque_free(*tokens, free_token), *tokens = NULL);
	(ast_free(*ast), free(*ast), *ast = NULL);
}

int	free_state(t_state *state, char **line, t_ddeque **tokens,
	t_ast **ast)
{
	free_datastructures(line, tokens, ast);
	(free(state->ps0), state->ps0 = NULL);
	(free(state->ps1), state->ps1 = NULL);
	return (1);
}

const char	*token_type_to_string(t_token_type token_type)
{
	if (token_type == TOK_OR)
		return (STR_TOK_OR);
	else if (token_type == TOK_OVERRIDE)
		return (STR_TOK_OVERRIDE);
	else if (token_type == TOK_INPUT)
		return (STR_TOK_INPUT);
	else if (token_type == TOK_APPEND)
		return (STR_TOK_APPEND);
	else if (token_type == TOK_HEREDOC)
		return (STR_TOK_HEREDOC);
	else if (token_type == TOK_PIPE)
		return (STR_TOK_PIPE);
	else if (token_type == TOK_AND)
		return (STR_TOK_AND);
	else if (token_type == TOK_OR)
		return (STR_TOK_OR);
	else if (token_type == TOK_L_PAREN)
		return (STR_TOK_L_PAREN);
	else if (token_type == TOK_R_PAREN)
		return (STR_TOK_R_PAREN);
	else if (token_type == TOK_SQUOTE_STR)
		return (STR_TOK_SQUOTE_STR);
	else if (token_type == TOK_DQUOTE_STR)
		return (STR_TOK_DQUOTE_STR);
	else if (token_type == TOK_WORD)
		return (STR_TOK_WORD);
	else if (token_type == TOK_EOL)
		return (STR_TOK_EOL);
	else if (token_type == TOK_ERROR)
		return (STR_TOK_ERROR);
	return (STR_TOK_UNKNOWN);
}

void	print_token(void *data, int first)
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

void	ast_print(t_ast *ast)
{
	print_token(ast->token, 1);
	ft_printf("\n");
}

/* TODO: what if readline returns NULL? */
/* TODO: remove DEBUG macros */
/* TODO: remove ddeque_print(tokens, print_token); */
int	main(void)
{
	static t_state	state;
	static int		i = 50;
	char			*line;
	t_ddeque		*tokens;
	t_ast			*ast;

	tokens = NULL;
	ast = NULL;
	while (i--)
	{
		update_state(&state);
		line = readline(state.ps1);
		if (!line && free_state(&state, &line, &tokens, &ast))
			break ;
		tokens = tokenize(line);
		ddeque_print(tokens, print_token);
		ast = parse(tokens);
		ast_print(ast);
		execute(ast);
		(void)free_state(&state, &line, &tokens, &ast);
	}
	return (0);
}
