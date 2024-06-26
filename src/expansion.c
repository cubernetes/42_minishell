#include "minishell.h"
#include "libft.h"
#include <math.h>

char	*var_lookup(char *key)
{
	t_var	*var;

	var = get_var(key);
	if (var && var->value)
		return (var->value);
	return ("");
}

/* static size_t	expand_var(char **str, char *var) */
/* { */
	/* char	*orig_var; */
	/* size_t	len; */
	/* char	*expanded_var; */
/*  */
	/* if (var[0] == '$' && (ft_isalpha(var[1]) || var[1] == '_')) */
	/* { */
		/* orig_var = ++var; */
		/* len = 0; */
		/* while (ft_isalpha(*var) || ft_isdigit(*var) || *var == '_') */
		/* { */
			/* ++var; */
			/* ++len; */
		/* } */
		/* expanded_var = var_lookup(ft_strndup(orig_var, len)); */
		/* *str = ft_strjoin(*str, expanded_var); */
		/* return (len + 1); */
	/* } */
	/* else if (var[0] == '$' && var[1] == '?') */
	/* { */
		/* expanded_var = var_lookup("?"); */
		/* *str = ft_strjoin(*str, ft_strdup(expanded_var)); */
		/* return (2); */
	/* } */
	/* *str = ft_strjoin(*str, ft_strndup(var, 1)); */
	/* return (1); */
/* } */
/*  */
/* static void	*new_word_token(char *str) */
/* { */
	/* return (new_token(str, TOK_WORD, true)); */
/* } */
/*  */
/* char	*expand_all_vars(char *token_str) */
/* { */
	/* char	*expanded_str; */
/*  */
	/* expanded_str = ""; */
	/* while (*token_str) */
		/* token_str += expand_var(&expanded_str, token_str); */
	/* return (expanded_str); */
/* } */

char	*get_ifs(void)
{
	t_var	*ifs_var;

	ifs_var = get_var("IFS");
	if (ifs_var == NULL || ifs_var->value == NULL)
		return (DEFAULT_IFS);
	return (ifs_var->value);
}

/* static char	*normalize_to_ifs0(char *str, char *ifs) */
/* { */
	/* size_t	ifs_len; */
	/* size_t	idx; */
	/* char	*ifs0; */
	/* size_t	prev_strlen; */
/*  */
	/* ifs_len = ft_strlen(ifs); */
	/* idx = 0; */
	/* ifs0 = ft_strndup(ifs, 1); */
	/* while (++idx < ifs_len) */
		/* str = ft_replace_all(str, ft_strndup(ifs + idx, 1), ifs0); */
	/* prev_strlen = ft_strlen(str); */
	/* while (1) */
	/* { */
		/* str = ft_replace(str, ft_strjoin(ifs0, ifs0), ifs0); */
		/* if (ft_strlen(str) == prev_strlen) */
			/* break ; */
		/* prev_strlen = ft_strlen(str); */
	/* } */
	/* return (ft_strtrim(str, ifs0)); */
/* } */
/*  */
/* static void	expand_word(t_list *new_tokens, t_token *token) */
/* { */
	/* t_list	*split_words; */
	/* t_list	*split_tokens; */
	/* t_var	*ifs; */
/*  */
	/* if (token->type != TOK_WORD) */
		/* return ; */
	/* ifs = get_var("IFS"); */
	/* if (ifs == NULL || ifs->value == NULL) */
		/* ifs = &(t_var){.value = DEFAULT_IFS}; */
	/* if (*ifs->value == '\0') */
		/* return ; */
	/* token->str = expand_all_vars(token->str); */
	/* token->str = normalize_to_ifs0(token->str, ifs->value); */
	/* split_tokens = liter(lsplit(token->str, ft_strndup(ifs->value, 1))); */
	/* split_words = lnew(); */
	/* while (lnext(split_tokens)) */
		/* lpush(split_words, as_token(new_word_token(split_tokens->current->as_str))); */
	/* lextend(new_tokens, split_words); */
	/* if (!token->is_last_token) */
		/* new_tokens->last->as_token->is_last_token = false; */
/* } */

/* char	*expand_parameters(const char *str) */
/* { */
	/* t_list	*new_tokens; */
	/* int		idx; */
	/* int		start; */
	/* int		num_tokens_after_split; */
/*  */
	/* num_tokens_after_split = 0; */
	/* new_tokens = lnew(); */
	/* if (*str == '\0') */
	/* { */
		/* lpush(new_tokens, as_token(new_token("", TOK_WORD, is_last_token))); */
		/* new_tokens->last->as_token->num_tokens_after_split */
			/* = num_tokens_after_split; */
		/* return (new_tokens); */
	/* } */
	/* idx = 0; */
	/* start = 0; */
	/* while (1) */
	/* { */
		/* if (str[idx] == '\0') */
			/* lpush(new_tokens, as_token(new_token(ft_strndup(str + start, idx - start), TOK_WORD, true))); // TODO: fix is_last_token logic */
		/* if (str[idx] == '$') */
			/* lpush(new_tokens, as_token(new_token(ft_strndup(str + start, idx - start), TOK_WORD, true))); // TODO: fix is_last_token logic */
		/* ++idx; */
	/* } */
	/* liter(new_tokens); */
	/* while (lnext(new_tokens)) */
		/* new_tokens->current->as_token->num_tokens_after_split */
			/* = num_tokens_after_split; */
	/* return (new_tokens); */
/* } */

// t_list	*expand_tokens(t_list *tokens)
// {
// 	t_list	*new_tokens;
// 	char	*expansion_ctx;
// 	t_token	*token;
// 
// 	new_tokens = lnew();
// 	liter(tokens);
// 	while (lnext(tokens))
// 	{
// 		token = tokens->current->as_token;
// 		if (token->type == TOK_DQUOTE_STR)
// 		{
// 			lpush(new_tokens, as_token(expand_dquote_str(token)));
// 		}
// 		else if (token->type == TOK_DQUOTE_STR)
// 		{
// 			token->str = expand_word(token->str, &expansion_ctx);
// 			token->expansion_ctx = expansion_ctx;
// 		}
// 		else
// 			lpush(new_tokens, as_data(tokens->current));
// 	}
// 	return (new_tokens);
// }

t_list	*remove_empty_words(t_list	*words)
{
	t_list	*new_words;

	new_words = lnew();
	liter(words);
	while (lnext(words))
		if (words->current->as_token->str[0] != '\0')
			lpush(new_words, as_data(words->current));
	if (words->first != NULL && new_words->len == 0)
		lpush(new_words, as_data(words->first));
	return (new_words);
}

t_list	*expand_subwords(t_list	*words)
{
	t_list	*new_words;
	t_token	*expanded_token;

	new_words = lnew();
	liter(words);
	while (lnext(words))
	{
		if (words->current->as_token->str[0] == '$' && words->current->as_token->escape_ctx[0] == '0')
		{
			if (words->current->as_token->str[1] == '\0')
				lpush(new_words, as_data(words->current));
			else
			{
				expanded_token = new_token(var_lookup(words->current->as_token->str + 1), TOK_WORD, true);
				expanded_token->expansion_ctx = repeat_string("1", ft_strlen(expanded_token->str));
				expanded_token->escape_ctx = repeat_string("0", ft_strlen(expanded_token->str) + 1);
				lpush(new_words, as_token(expanded_token));
			}
		}
		else
		{
			words->current->as_token->expansion_ctx = repeat_string("0", ft_strlen(words->current->as_token->str));
			lpush(new_words, as_data(words->current));
		}
	}
	return (new_words);
}

t_list	*split_into_subwords(t_token *token)
{
	t_list	*subwords;
	int		idx;
	int		start;
	int		parse_param;
	t_token	*tok;

	subwords = lnew();
	idx = -1;
	start = 0;
	parse_param = 0;
	while (token->str[++idx])
	{
		if (token->str[idx] == '$' && parse_param == 0 && token->escape_ctx[idx] == '0')
		{
			tok = new_token(ft_strndup(token->str + start, (size_t)(idx - start)), TOK_WORD, true);
			tok->escape_ctx = ft_strndup(token->escape_ctx + start, (size_t)(idx - start + 1));
			lpush(subwords, as_token(tok));
			start = idx;
			parse_param = 1;
		}
		else if (parse_param > 0)
		{
			if (token->escape_ctx[idx] == '1')
				parse_param = 0;
			if (parse_param == 1 && (ft_isalpha(token->str[idx]) || token->str[idx] == '_'))
				++parse_param;
			else if (parse_param == 1 && ft_strchr("?$-0123456789@*#", token->str[idx]))
			{
				tok = new_token(ft_strndup(token->str + start, (size_t)(idx - start + 1)), TOK_WORD, true);
				tok->escape_ctx = ft_strndup(token->escape_ctx + start, (size_t)(idx - start + 2));
				lpush(subwords, as_token(tok));
				start = idx + 1;
				parse_param = 0;
			}
			else if (parse_param > 1 && (ft_isalpha(token->str[idx]) || ft_isdigit(token->str[idx]) || token->str[idx] == '_'))
				++parse_param;
			else
			{
				tok = new_token(ft_strndup(token->str + start, (size_t)(idx - start)), TOK_WORD, true);
				tok->escape_ctx = ft_strndup(token->escape_ctx + start, (size_t)(idx - start + 1));
				lpush(subwords, as_token(tok));
				start = idx;
				parse_param = 0;
			}
		}
	}
	tok = new_token(ft_strndup(token->str + start, (size_t)(idx - start)), TOK_WORD, true);
	tok->escape_ctx = ft_strndup(token->escape_ctx + start, (size_t)(idx - start + 1));
	lpush(subwords, as_token(tok));
	return (subwords);
}

/* start inclusive, end exclusive */
static t_token	*new_word_token(t_token *token, int start, int end)
{
	t_token	*new_tok;

	new_tok = new_token(ft_strndup(token->str + start, (size_t)(end - start)), TOK_WORD, true);
	new_tok->quoting_ctx = ft_strndup(token->quoting_ctx + start, (size_t)(end - start));
	new_tok->expansion_ctx = ft_strndup(token->expansion_ctx + start, (size_t)(end - start));
	new_tok->escape_ctx = ft_strndup(token->escape_ctx + start, (size_t)(end - start) + 1);
	return (new_tok);
}

/* shit... */
/* probably the most complex function in the entire codebase */
t_list	*split_into_words(t_token *token)
{
	t_list	*words;
	char	*ifs;
	int		idx;
	int		start;
	bool	push;

	words = lnew();
	ifs = get_ifs();
	idx = -1;
	start = -1;
	push = false;
	while (token->str[++idx])
	{
		if (token->expansion_ctx[idx] == '1')
		{
			if (ft_strchr(ifs, token->str[idx]))
			{
				if (ft_strchr(IFS_WHITESPACE, token->str[idx]))
				{
					if (push == true)
					{
						lpush(words, as_token(new_word_token(token, start, idx)));
						push = false;
					}
				}
				else
				{
					if (push == true)
					{
						lpush(words, as_token(new_word_token(token, start, idx)));
						start = idx;
						push = false;
					}
					else
					{
						if (start == -1 || (ft_strchr(ifs, token->str[start]) && !ft_strchr(IFS_WHITESPACE, token->str[start])))
							lpush(words, as_token(new_token("", TOK_WORD, true)));
						else
							start = idx;
					}
				}
			}
			else
			{
				if (push == false)
				{
					start = idx;
					push = true;
				}
			}
		}
		else
		{
			if (push == false)
			{
				start = idx;
				push = true;
			}
		}
	}
	if (start != -1 && push == true && (token->expansion_ctx[start] == '0' || !ft_strchr(ifs, token->str[start])))
		lpush(words, as_token(new_word_token(token, start, idx)));
	if (words->len == 0)
	{
		token = new_token("", TOK_WORD, true);
		token->num_tokens_after_split = 0;
		lpush(words, as_token(token));
	}
	else
	{
		liter(words);
		while (lnext(words))
			words->current->as_token->num_tokens_after_split = (int)words->len;
	}
	return (words);
}

t_list	*expand_tokens(t_list *tokens)
{
	t_token	*joined_token;
	t_list	*subwords;
	t_list	*new_tokens;
	t_list	*split_tokens;

	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
	{
		if (tokens->current->as_token->type != TOK_WORD && tokens->current->as_token->type != TOK_DQUOTE_STR)
		{
			lpush(new_tokens, as_data(tokens->current));
			continue ;
		}
		subwords = split_into_subwords(tokens->current->as_token);
		subwords = expand_subwords(subwords);
		subwords = liter(remove_empty_words(subwords));
		joined_token = new_token("", TOK_WORD, true);
		joined_token->expansion_ctx = "";
		joined_token->escape_ctx = "";
		while (lnext(subwords))
		{
			joined_token->str = ft_strjoin(joined_token->str, subwords->current->as_token->str);
			joined_token->expansion_ctx = ft_strjoin(joined_token->expansion_ctx, subwords->current->as_token->expansion_ctx);
			joined_token->quoting_ctx = ft_strjoin(joined_token->quoting_ctx, subwords->current->as_token->quoting_ctx);
			joined_token->escape_ctx = ft_strjoin(ft_strndup(joined_token->escape_ctx, ft_strlen(joined_token->escape_ctx) - 1), subwords->current->as_token->escape_ctx);
		}
		if (tokens->current->as_token->type == TOK_WORD)
		{
			if (joined_token->str[0] != '\0' && ft_strchr(get_ifs(), joined_token->str[0]))
				if (new_tokens->last != NULL && new_tokens->last->as_token->is_last_token == false)
					new_tokens->last->as_token->is_last_token = true;
			split_tokens = liter(split_into_words(joined_token));
			while (lnext(split_tokens))
				split_tokens->current->as_token->origin = tokens->current->as_token->origin;
			lextend(new_tokens, lcopy(split_tokens));
			if (split_tokens->last->as_token->num_tokens_after_split == 0
				|| !ft_strchr(get_ifs(), joined_token->str[ft_strlen(joined_token->str) - 1])
				|| joined_token->escape_ctx[ft_strlen(joined_token->str) - 1] == '1')
				if (tokens->current->as_token->is_last_token == false)
					new_tokens->last->as_token->is_last_token = false;
		}
		else
		{
			joined_token->type = TOK_DQUOTE_STR;
			joined_token->quoting_ctx = repeat_string("1", ft_strlen(joined_token->str));
			joined_token->origin = tokens->current->as_token->origin;
			lpush(new_tokens, as_token(joined_token));
			if (tokens->current->as_token->is_last_token == false)
				new_tokens->last->as_token->is_last_token = false;
		}
	}
	return (new_tokens);
}
