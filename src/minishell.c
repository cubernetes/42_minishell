/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/25 23:41:33 by tosuman          ###   ########.fr       */
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
void	*new_token(const char *line, size_t token_len, t_token_type token_type)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (!token)
		internal_error(ERR_NEW_TOKEN, __LINE__);
	token->token = ft_strndup(line, token_len + 1);
	token->token[token_len] = 0;
	token->token_type = token_type;
	return (token);
}

int	push_token(const char **line, t_ddeque *tokens, size_t token_len,
	t_token_type token_type)
{
	ddeque_push_value_bottom(tokens, new_token(*line, token_len, token_type));
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
		pushed = push_token(line, tokens, 2, TOK_OVERRIDE);
	else if (**line == '<' && *(*line + 1) == '<')
		pushed = push_token(line, tokens, 2, TOK_APPEND);
	else if (**line == '&' && *(*line + 1) == '&')
		pushed = push_token(line, tokens, 2, TOK_AND);
	else if (**line == '|' && *(*line + 1) == '|')
		pushed = push_token(line, tokens, 2, TOK_OR);
	else if (**line == '>')
		pushed = push_token(line, tokens, 1, TOK_HEREDOC);
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

void	tokenize_variable_len_tokens(const char **line, t_ddeque *tokens)
{
	size_t	len;

	len = 1;
	if (**line == '\'')
	{
		while (**line != '\'' && **line != '\0' && ++(*line))
			++len;
		if (**line == '\'')
			push_token(line, tokens, len + 1, TOK_SQOUTE_STR);
		else
			push_token(line, tokens, len, TOK_ERROR);
	}
	else if (**line == '"')
	{
		while (**line != '"' && **line != '\0' && ++(*line))
			++len;
		if (**line == '"')
			push_token(line, tokens, len + 1, TOK_SQOUTE_STR);
		else
			push_token(line, tokens, len, TOK_ERROR);
	}
	else if (ft_isprint(**line) && !ft_isspace(**line)
		&& !ft_strchr("><&|()'\"", **line))
	{
		while (ft_isprint(**line) && !ft_isspace(**line)
			&& !ft_strchr("><&|()'\"", **line))
			++len;
		push_token(line, tokens, len, TOK_WORD);
	}
	else
		internal_error("Lexingerror.", __LINE__);
}

/* environment variable expansion is NOT happening in the tokenization phase*/
/* wildcard expansion (globbing) is also not happening here*/
/* those are responsibilities while or after building the AST */
t_ddeque	*tokenize(const char *line)
{
	t_ddeque		*tokens;

	tokens = ddeque_init();
	while (*line)
	{
		skip_whitespace(&line);
		if (!tokenize_fixed_len_tokens(&line, tokens))
			tokenize_variable_len_tokens(&line, tokens);
	}
	return (tokens);
}

t_ast	*parse(t_ddeque *tokens)
{
	t_ast	*ast;

	(void)tokens;
	ast = NULL;
	return (ast);
}

void	execute(t_ast *ast)
{
	(void)ast;
}

void	free_token(void *data)
{
	t_token	*token;

	token = (t_token *)data;
	free(token->token);
	free(token);
}

void	free_state(t_state *state)
{
	free(state->ps0);
	free(state->ps1);
}

void	ast_free(void *data)
{
	t_ast	*ast;

	ast = (t_ast *)data;
	free(ast->token);
	while (*ast->ast_children)
	{
		ast_free(*ast->ast_children);
		++ast->ast_children;
	}
}

void	free_datastructures(char *line, t_ddeque *tokens, t_ast *ast)
{
	free(line);
	ddeque_free(tokens, free_token);
	ast_free(ast);
}

/* TODO: what if readline returns NULL? */
/* TODO: remove DEBUG macros */
int	main(void)
{
	static t_state	state;
	static int		i = 2;
	char			*line;
	t_ddeque		*tokens;
	t_ast			*ast;

	while (i--)
	{
		DEBUG(0, "%s", "begin loop");
		update_state(&state);
		DEBUG(0, "%s", "state updating done");
		line = readline(state.ps1);
		DEBUG(0, "%s", "readline done");
		tokens = tokenize(line);
		DEBUG(0, "%s", "tokenization done");
		ast = parse(tokens);
		DEBUG(0, "%s", "parsing done");
		execute(ast);
		DEBUG(0, "%s", "execution done");
		free_datastructures(line, tokens, ast);
		DEBUG(0, "%s", "datastructure freeing done");
		free_state(&state);
		DEBUG(0, "%s", "state freeing done");
	}
	(void)line;
	return (0);
}
