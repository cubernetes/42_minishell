#include "../../include/minishell.h"
#include "../../libft/libft.h"
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

char	*get_token_str(t_deque *tokens)
{
	return (tokens->head->as_token->str);
}

char	*get_token_str_nl(t_deque *tokens)
{
	char	*str;

	str = get_token_str(tokens);
	if (!*str)
		str = "newline";
	return (str);
}

t_token_type	get_token_type(t_deque *tokens)
{
	return (tokens->head->as_token->type);
}

/* TODO: Not required: hashtable */
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

void	skip_whitespace(const char **line)
{
	while (ft_isspace(**line))
		++(*line);
}

void	print_token(void *ptr, t_bool first)
{
	t_token	*token;
	char	*clr;

	token = (t_token *)ptr;
	if (token->is_last_subtoken)
		clr = "";
	else
		clr = "\033[33m";
	if (first)
		ft_printf("<\033[31m%s\033[m> (%s%s\033[m)", token->str, clr,
			token_type_to_string(token->type));
	else
		ft_printf(" -> <\033[31m%s\033[m> (%s%s\033[m)", token->str, clr,
			token_type_to_string(token->type));
}

/* TODO: change return type to t_token later */
void	*new_token(char *str, t_token_type type, t_bool is_last_subtoken)
{
	return (ft_memdup(\
		&(t_token){
			.str = str,
			.type = type,
			.is_last_subtoken = is_last_subtoken
		},
		sizeof(t_token)
	));
}

t_bool	push_token(const char **line, t_deque *tokens, size_t token_len,
	t_token_type type)
{
	char	*str;
	t_bool	is_last_subtoken;
	t_bool	is_str_tok;
	t_bool	is_last;
	t_bool	is_last_quote;

	str = ft_strndup(*line, token_len);
	is_str_tok = type == TOK_SQUOTE_STR || type == TOK_DQUOTE_STR
		|| type == TOK_WORD;
	is_last = ft_isspace((*line)[token_len]) || type == TOK_EOL
		|| (*line)[token_len] == '\0';
	is_last_quote = ((*line)[token_len] == '\'' || (*line)[token_len] == '"')
		&& (*line)[token_len + 1] == '\0';
	if (is_last || is_last_quote || !is_str_tok)
		is_last_subtoken = TRUE;
	else
		is_last_subtoken = FALSE;
	deque_push_ptr_right(tokens, new_token(str, type, is_last_subtoken));
	*line += token_len;
	return (TRUE);
}

/* TODO: Not required: hashtable */
t_bool	tokenize_fixed_len_tokens(const char **line, t_deque *tokens)
{
	t_bool	pushed;

	pushed = FALSE;
	if (**line == '\0')
		pushed = push_token(line, tokens, 0, TOK_EOL);
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

t_bool	tokenize_single_quoted_string(const char **line, t_deque *tokens)
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

t_bool	tokenize_double_quoted_string(const char **line, t_deque *tokens)
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

t_bool	tokenize_word(const char **line, t_deque *tokens)
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

void	tokenize_variable_len_tokens(const char **line, t_deque *tokens)
{
	t_bool	pushed;

	pushed = FALSE;
	pushed += tokenize_single_quoted_string(line, tokens);
	pushed += tokenize_double_quoted_string(line, tokens);
	pushed += tokenize_word(line, tokens);
	if (!pushed)
	{
		minishell_error(EXIT_FAILURE, "could not tokenize `%s'",
			get_token_str(tokens));
	}
}

/* environment variable expansion is NOT happening in the tokenization phase*/
/* wildcard expansion (globbing) is also not happening here*/
/* those are responsibilities while or after building the AST */
/* TODO: well, I think you kinda have to do env expansion and globbing here.. */
/* TODO: set is_last_subtoken member for each token */
t_deque	*tokenize(const char *line)
{
	t_deque	*tokens;

	tokens = deque_init();
	skip_whitespace(&line);
	while (TRUE)
	{
		if (!tokenize_fixed_len_tokens(&line, tokens))
			tokenize_variable_len_tokens(&line, tokens);
		if (tokens->head->prev->as_token->type == TOK_EOL)
			break ;
		skip_whitespace(&line);
	}
	expand_env_vars(tokens);
	glob_tokens(tokens);
	join_tokens(tokens);
	return (tokens);
}
