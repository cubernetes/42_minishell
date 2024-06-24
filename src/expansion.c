#include "minishell.h"
#include "libft.h"

char	*var_lookup(char *key)
{
	t_var	*var;

	var = get_var(key);
	if (var && var->value)
		return (var->value);
	return ("");
}

static size_t	expand_var(char **str, char *var)
{
	char	*orig_var;
	size_t	len;
	char	*expanded_var;

	if (var[0] == '$' && (ft_isalpha(var[1]) || var[1] == '_'))
	{
		orig_var = ++var;
		len = 0;
		while (ft_isalpha(*var) || ft_isdigit(*var) || *var == '_')
		{
			++var;
			++len;
		}
		expanded_var = var_lookup(ft_strndup(orig_var, len));
		*str = ft_strjoin(*str, expanded_var);
		return (len + 1);
	}
	else if (var[0] == '$' && var[1] == '?')
	{
		expanded_var = var_lookup("?");
		*str = ft_strjoin(*str, ft_strdup(expanded_var));
		return (2);
	}
	*str = ft_strjoin(*str, ft_strndup(var, 1));
	return (1);
}

static void	*new_word_token(char *str)
{
	return (new_token(str, TOK_WORD, true));
}

char	*expand_all_vars(char *token_str)
{
	char	*expanded_str;

	expanded_str = "";
	while (*token_str)
		token_str += expand_var(&expanded_str, token_str);
	return (expanded_str);
}

char	*get_ifs(void)
{
	t_var	*ifs_var;

	ifs_var = get_var("ifs_var");
	if (ifs_var == NULL || ifs_var->value == NULL)
		return (DEFAULT_IFS);
	if (*ifs_var->value == '\0')
		return ("");
	return (ifs_var->value);
}

static char	*normalize_to_ifs0(char *str, char *ifs)
{
	size_t	ifs_len;
	size_t	idx;
	char	*ifs0;
	size_t	prev_strlen;

	ifs_len = ft_strlen(ifs);
	idx = 0;
	ifs0 = ft_strndup(ifs, 1);
	while (++idx < ifs_len)
		str = ft_replace_all(str, ft_strndup(ifs + idx, 1), ifs0);
	prev_strlen = ft_strlen(str);
	while (1)
	{
		str = ft_replace(str, ft_strjoin(ifs0, ifs0), ifs0);
		if (ft_strlen(str) == prev_strlen)
			break ;
		prev_strlen = ft_strlen(str);
	}
	return (ft_strtrim(str, ifs0));
}

static void	expand_word(t_list *new_tokens, t_token *token)
{
	t_list	*split_words;
	t_list	*split_tokens;
	t_var	*ifs;

	if (token->type != TOK_WORD)
		return ;
	ifs = get_var("IFS");
	if (ifs == NULL || ifs->value == NULL)
		ifs = &(t_var){.value = DEFAULT_IFS};
	if (*ifs->value == '\0')
		return ;
	token->str = expand_all_vars(token->str);
	token->str = normalize_to_ifs0(token->str, ifs->value);
	split_tokens = liter(lsplit(token->str, ft_strndup(ifs->value, 1)));
	split_words = lnew();
	while (lnext(split_tokens))
		lpush(split_words, as_token(new_word_token(split_tokens->current->as_str)));
	lextend(new_tokens, split_words);
	if (!token->is_last_token)
		new_tokens->last->as_token->is_last_token = false;
}

char	*expand_parameters(const char *str)
{
	t_list	*new_tokens;
	int		idx;
	int		start;
	int		num_tokens_after_split;

	num_tokens_after_split = 0;
	new_tokens = lnew();
	if (*str == '\0')
	{
		lpush(new_tokens, as_token(new_token("", TOK_WORD, is_last_token)));
		new_tokens->last->as_token->num_tokens_after_split
			= num_tokens_after_split;
		return (new_tokens);
	}
	idx = 0;
	start = 0;
	while (1)
	{
		if (str[idx] == '\0')
			lpush(new_tokens, as_token(new_token(ft_strndup(str + start, idx - start), TOK_WORD, true))); // TODO: fix is_last_token logic
		if (str[idx] == '$')
			lpush(new_tokens, as_token(new_token(ft_strndup(str + start, idx - start), TOK_WORD, true))); // TODO: fix is_last_token logic
		++idx;
	}
	liter(new_tokens);
	while (lnext(new_tokens))
		new_tokens->current->as_token->num_tokens_after_split
			= num_tokens_after_split;
	return (new_tokens);
}

t_list	*expand_tokens(t_list *tokens)
{
	t_list	*new_tokens;
	char	*split_ctx;

	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
	{
		if (tokens->current->as_token->type == TOK_WORD || tokens->current->as_token->type == TOK_DQUOTE_STR)
		{
			tokens->current->as_token->str = expand_parameters(tokens->current->as_token->str, &split_ctx);
			tokens->current->as_token->split_ctx = split_ctx;
		}
		else
			lpush(new_tokens, as_data(tokens->current));
	}
	return (new_tokens);
}
