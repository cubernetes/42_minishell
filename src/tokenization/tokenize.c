#include "minishell.h"
#include "libft.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

char	*get_token_str(t_list *tokens)
{
	return (tokens->first->as_token->str);
}

char	*get_token_str_nl(t_list *tokens)
{
	char	*str;

	str = get_token_str(tokens);
	if (!*str)
		str = "newline";
	return (str);
}

t_token_type	get_token_type(t_list *tokens)
{
	if (tokens->first->as_token->type == TOK_SQUOTE_STR
		|| tokens->first->as_token->type == TOK_DQUOTE_STR)
		return (TOK_WORD);
	else if (tokens->first->as_token->type == TOK_SEMI)
		return (TOK_AND);
	else if (tokens->first->as_token->type == TOK_OVERRIDE_ERR)
		return (TOK_OVERRIDE);
	else if (tokens->first->as_token->type == TOK_APPEND_ERR)
		return (TOK_APPEND);
	return (tokens->first->as_token->type);
}

/* TODO: Not required: hashtable */
const char	*token_type_to_string(t_token_type type)
{
	if (type == TOK_OVERRIDE)
		return (STR_TOK_OVERRIDE);
	if (type == TOK_OVERRIDE_ERR)
		return (STR_TOK_OVERRIDE_ERR);
	else if (type == TOK_INPUT)
		return (STR_TOK_INPUT);
	else if (type == TOK_APPEND)
		return (STR_TOK_APPEND);
	else if (type == TOK_APPEND_ERR)
		return (STR_TOK_APPEND_ERR);
	else if (type == TOK_HEREDOC)
		return (STR_TOK_HEREDOC);
	else if (type == TOK_PIPE)
		return (STR_TOK_PIPE);
	else if (type == TOK_AND)
		return (STR_TOK_AND);
	else if (type == TOK_OR)
		return (STR_TOK_OR);
	else if (type == TOK_SEMI)
		return (STR_TOK_SEMI);
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

void	skip_space_tab(const char **line)
{
	while (**line == ' ' || **line == '\t')
		++(*line);
}

void	print_token_debug(t_data data, int n)
{
	t_token	*token;
	char	*clr;
	char	*sep;

	/* sep = "\t|\t"; */
	sep = "|";
	token = data.as_token;
	if (token->is_last_token)
		clr = "";
	else
		clr = "\033[33m";
	if (n == 0)
		ft_printf("<\033[31m%s\033[m>%s%s%s\033[m%s%d%s<%s>%s<%s>%s<%s>%s<%s>", token->str, sep, clr,
			token_type_to_string(token->type), sep, token->num_tokens_after_split, sep, token->expansion_ctx, sep, token->quoting_ctx, sep, token->escape_ctx, sep, token->origin);
	else
		ft_printf("\n<\033[31m%s\033[m>%s%s%s\033[m%s%d%s<%s>%s<%s>%s<%s>%s<%s>", token->str, sep, clr,
			token_type_to_string(token->type), sep, token->num_tokens_after_split, sep, token->expansion_ctx, sep, token->quoting_ctx, sep, token->escape_ctx, sep, token->origin);
}

void	print_token(t_data data, int n)
{
	t_token	*token;
	char	*clr;

	token = data.as_token;
	if (token->is_last_token)
		clr = "";
	else
		clr = "\033[33m";
	if (n == 0)
		ft_printf("<\033[31m%s\033[m> (%s%s\033[m)", token->str, clr,
			token_type_to_string(token->type));
	else
		ft_printf(" -> <\033[31m%s\033[m> (%s%s\033[m)", token->str, clr,
			token_type_to_string(token->type));
}

static char	*was_quoted(t_token_type type)
{
	if (type == TOK_SQUOTE_STR || type == TOK_DQUOTE_STR)
		return ("1");
	return ("0");
}

char	*repeat_string(char *str, size_t n)
{
	char	*res;

	res = "";
	while (n--)
		res = ft_strjoin(res, str);
	return (res);
}

/* TODO: change return type to t_token later */
void	*new_token(char *str, t_token_type type, bool is_last_token)
{
	int	s;

	s = 1;
	if (type == TOK_EOL)
		s = 0;
	return (ft_memdup(\
		&(t_token){
			.str = str,
			.type = type,
			.is_last_token = is_last_token,
			.quoting_ctx = repeat_string(was_quoted(type), ft_strlen(str)),
			.expansion_ctx = repeat_string("0", ft_strlen(str)),
			.num_tokens_after_split = s,
			.escape_ctx = repeat_string("0", ft_strlen(str) + 1),
			.origin = ""
		},
		sizeof(t_token)
	));
}

static char	*process_backslashes(char *str, t_token_type type, char **escape_ctx)
{
	t_list	*result_chars;
	t_list	*escape_ctx_chars;

	result_chars = lnew();
	escape_ctx_chars = lnew();
	if (type == TOK_DQUOTE_STR)
	{
		while (*str)
		{
			if (*str == '\\')
			{
				if (str[1])
				{
					if (!ft_strchr("\"$\\", str[1]))
					{
						lpush(result_chars, as_str("\\"));
						lpush(escape_ctx_chars, as_str("0"));
					}
					lpush(escape_ctx_chars, as_str("1"));
					lpush(result_chars, as_str(ft_strndup(str + 1, 1)));
					++str;
				}
				else
				{
					lpush(escape_ctx_chars, as_str("1"));
					// TODO: Handle line continuations
				}
			}
			else
			{
				lpush(result_chars, as_str(ft_strndup(str, 1)));
				lpush(escape_ctx_chars, as_str("0"));
			}
			++str;
		}
		if (escape_ctx_chars->len == result_chars->len)
			lpush(escape_ctx_chars, as_str("0"));
		*escape_ctx = ljoin(escape_ctx_chars, "");
		return (ljoin(result_chars, ""));
	}
	else if (type == TOK_WORD)
	{
		while (*str)
		{
			if (*str == '\\')
			{
				if (str[1])
				{
					lpush(result_chars, as_str(ft_strndup(str + 1, 1)));
					lpush(escape_ctx_chars, as_str("1"));
					++str;
				}
				else
				{
					lpush(escape_ctx_chars, as_str("1"));
					// TODO: Handle line continuations
				}
			}
			else
			{
				lpush(result_chars, as_str(ft_strndup(str, 1)));
				lpush(escape_ctx_chars, as_str("0"));
			}
			++str;
		}
		if (escape_ctx_chars->len == result_chars->len)
			lpush(escape_ctx_chars, as_str("0"));
		*escape_ctx = ljoin(escape_ctx_chars, "");
		return (ljoin(result_chars, ""));
	}
	else
		return (str);
}

bool	push_token(const char **line, t_list *tokens, size_t token_len,
	t_token_type type)
{
	char	*str;
	bool	is_last_token;
	bool	is_str_tok;
	bool	is_last;
	bool	is_last_quote;
	t_token	*token;
	char	*escape_ctx;

	str = process_backslashes(ft_strndup(*line, token_len), type, &escape_ctx);
	is_str_tok = type == TOK_SQUOTE_STR || type == TOK_DQUOTE_STR
		|| type == TOK_WORD;
	is_last = ft_isspace((*line)[token_len]) || type == TOK_EOL
		|| (*line)[token_len] == '\0';
	if (type == TOK_SQUOTE_STR || type == TOK_DQUOTE_STR)
		is_last_quote = ((*line)[token_len + 1] == '\0' || ft_isspace((*line)[token_len + 1]));
	else
		is_last_quote = false;
	if (is_last || is_last_quote || !is_str_tok)
		is_last_token = true;
	else
		is_last_token = false;
	token = new_token(str, type, is_last_token);
	token->escape_ctx = escape_ctx;
	token->origin = ft_strndup(*line, token_len);
	lpush(tokens, as_token(token));
	*line += token_len;
	return (true);
}

/* TODO: Not required: hashtable */
bool	tokenize_fixed_len_tokens(const char **line, t_list *tokens)
{
	bool	pushed;

	pushed = false;
	if (**line == '\0')
		pushed = push_token(line, tokens, 0, TOK_EOL);
	else if (**line == '2' && *(*line + 1) == '>' && *(*line + 2) == '>')
		pushed = push_token(line, tokens, 3, TOK_APPEND_ERR);
	else if (**line == '2' && *(*line + 1) == '>')
		pushed = push_token(line, tokens, 2, TOK_OVERRIDE_ERR);
	else if (**line == '>' && *(*line + 1) == '>')
		pushed = push_token(line, tokens, 2, TOK_APPEND);
	else if (**line == '<' && *(*line + 1) == '<')
		pushed = push_token(line, tokens, 2, TOK_HEREDOC);
	else if (**line == '&' && *(*line + 1) == '&')
		pushed = push_token(line, tokens, 2, TOK_AND);
	else if (**line == '|' && *(*line + 1) == '|')
		pushed = push_token(line, tokens, 2, TOK_OR);
	else if (**line == ';')
		pushed = push_token(line, tokens, 1, TOK_SEMI);
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

bool	tokenize_single_quoted_string(const char **line, t_list *tokens)
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
		return (true);
	}
	return (false);
}

bool	tokenize_double_quoted_string(const char **line, t_list *tokens)
{
	size_t		len;
	const char	*tmp;
	bool		escaped;

	if (**line == '"')
	{
		len = 0;
		tmp = *line + 1;
		escaped = false;
		while ((escaped || *tmp != '"') && *tmp != '\0')
		{
			if (escaped == true)
				escaped = false;
			else if (*tmp == '\\')
				escaped = true;
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
		return (true);
	}
	return (false);
}

/* Not including `&' since we do not have job control */
bool	is_word_char(char c)
{
	return ((bool)(
		c
		&& c != ' ' && c != '\t'
		&& !ft_strchr(";><()'\"|", c)
	));
}

bool	is_not_and_and(const char *line)
{
	if (line[0] == '&' && line[1] == '&')
		return (false);
	return (true);
}

bool	tokenize_word(const char **line, t_list *tokens)
{
	size_t		len;
	const char	*tmp;
	bool		escaped;

	if (is_word_char(**line) && is_not_and_and(*line))
	{
		len = 0;
		tmp = *line;
		escaped = false;
		while ((escaped || (is_word_char(*tmp) && is_not_and_and(tmp))) && *tmp != '\0')
		{
			if (escaped == true)
				escaped = false;
			else if (*tmp == '\\')
				escaped = true;
			++tmp;
			++len;
		}
		push_token(line, tokens, len, TOK_WORD);
		return (true);
	}
	return (false);
}

bool	tokenize_variable_len_tokens(const char **line, t_list *tokens)
{
	bool	pushed;

	pushed = false;
	pushed |= tokenize_single_quoted_string(line, tokens);
	pushed |= tokenize_double_quoted_string(line, tokens);
	pushed |= tokenize_word(line, tokens);
	if (!pushed)
		return (minishell_error(EXIT_FAILURE, false, false, "could not tokenize `%s'",
			get_token_str(tokens)), false);
	return (true);
}

/* line will never contain a newline */
/* context = 0 -> not quoted */
/* context = 1 -> double quoted */
/* context = 2 -> single quoted */
/* static char	*process_backslashes_old(const char *line) */
/* { */
	/* int		context; */
	/* t_list	*result_chars; */
	/* int		idx; */
/*  */
	/* context = 0; */
	/* result_chars = lnew(); */
	/* idx = -1; */
	/* while (line[++idx]) */
	/* { */
		/* if (line[idx] == '\\') */
		/* { */
			/* if (context == 2) */
				/* lpush(result_chars, as_str("\\")); */
			/* else if (context == 1) */
			/* { */
				/* if (ft_strchr("\"$\\", line[idx + 1])) */
				/* { */
					/* lpush(result_chars, as_str("\"'")); */
					/* lpush(result_chars, as_str(ft_strndup(&line[idx + 1], 1))); */
					/* lpush(result_chars, as_str("'\"")); */
					/* ++idx; */
				/* } */
				/* else */
					/* lpush(result_chars, as_str("\\")); */
			/* } */
			/* else */
			/* { */
				/* if (line[idx + 1] == '\'') */
					/* lpush(result_chars, as_str("\"'\"")); */
				/* else */
				/* { */
					/* lpush(result_chars, as_str("'")); */
					/* lpush(result_chars, as_str(ft_strndup(&line[idx + 1], 1))); */
					/* lpush(result_chars, as_str("'")); */
				/* } */
				/* ++idx; */
			/* } */
		/* } */
		/* else */
		/* { */
			/* lpush(result_chars, as_str(ft_strndup(&line[idx], 1))); */
			/* if (context == 0 && line[idx] == '\'') */
				/* context = 2; */
			/* else if (context == 0 && line[idx] == '"') */
				/* context = 1; */
			/* else if (context == 1 && line[idx] == '"') */
				/* context = 0; */
			/* else if (context == 2 && line[idx] == '\'') */
				/* context = 0; */
		/* } */
	/* } */
	/* return (ljoin(result_chars, "")); */
/* } */

/* TODO: set is_last_token member for each token */
t_list	*tokenize(const char *line)
{
	t_list	*tokens;

	/* line = process_backslashes_old(line); */
	tokens = lnew();
	skip_space_tab(&line);
	while (true)
	{
		if (!tokenize_fixed_len_tokens(&line, tokens))
			if (!tokenize_variable_len_tokens(&line, tokens))
				return (NULL);
		if (tokens->last->as_token->type == TOK_EOL)
			break ;
		skip_space_tab(&line);
	}
	return (tokens);
}
