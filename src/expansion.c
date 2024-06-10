#include "minishell.h"
#include "libft.h"

char	*var_lookup(char *key)
{
	t_var	*var;

	var = get_var(key);
	if (var)
		return (var->value);
	return ("");
}

static size_t	expand_vars(t_token *token, char *var)
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
		token->str = ft_strjoin(token->str, expanded_var);
		return (len + 1);
	}
	else if (var[0] == '$' && var[1] == '?')
	{
		expanded_var = var_lookup("?");
		token->str = ft_strjoin(token->str, ft_strdup(expanded_var));
		return (2);
	}
	token->str = ft_strjoin(token->str, ft_strndup(var, 1));
	return (1);
}

static void	*new_word_token(char *str)
{
	return (new_token(str, TOK_WORD, true));
}

/* TODO: Not required: use full IFS instead of just the first char */
static void	expand_word(t_list *new_tokens, t_token *token)
{
	char	*token_str;
	t_list	*split_words;
	t_list	*split_tokens;

	if (token->type != TOK_WORD)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	split_words = lnew();
	split_tokens = liter(lsplit(token->str, ft_strndup(IFS, 1)));
	while (lnext(split_tokens))
		lpush(split_words, as_token(new_word_token(split_tokens->current->as_str)));
	lextend(new_tokens, split_words);
	if (!token->is_last_subtoken)
		new_tokens->last->as_token->is_last_subtoken = false;
}

static void	expand_dquote_str(t_list *new_tokens, t_token *token)
{
	char	*token_str;

	if (token->type != TOK_DQUOTE_STR)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	lpush(new_tokens, as_token(token));
}

static void	expand(t_list *new_tokens, t_token *token)
{
	if (token->type == TOK_WORD)
		expand_word(new_tokens, token);
	else if (token->type == TOK_DQUOTE_STR)
		expand_dquote_str(new_tokens, token);
	else
		lpush(new_tokens, as_token(token));
}

t_list	*expand_token(t_token *token)
{
	t_list	*new_tokens;

	new_tokens = lnew();
	if (token->type == TOK_WORD)
		expand_word(new_tokens, token);
	else if (token->type == TOK_DQUOTE_STR)
		expand_dquote_str(new_tokens, token);
	else
		lpush(new_tokens, as_token(token));
	return (new_tokens);
}

void	expand_env_vars(t_list *tokens)
{
	t_list	*new_tokens;

	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
		expand(new_tokens, tokens->current->as_token);
	tokens->first = new_tokens->first;
	tokens->last = new_tokens->last;
	tokens->len = new_tokens->len;
}

void	join_tokens(t_list *tokens)
{
	t_list_node	*first;
	t_list		*new_tokens;
	t_token		*token;
	t_token		*word_token;

	new_tokens = lnew();
	first = tokens->first;
	if (!first)
		return ;
	token = first->as_token;
	word_token = NULL;
	if (token->is_last_subtoken)
		lpush(new_tokens, as_token(token));
	else
		word_token = new_token(token->str, TOK_WORD, true);
	while (first->next != tokens->first)
	{
		first = first->next;
		token = first->as_token;
		if (!token->is_last_subtoken)
		{
			if (!word_token)
				word_token = new_token("", TOK_WORD, true);
			word_token->str = ft_strjoin(word_token->str, token->str);
			word_token->quoting_info = ft_strjoin(word_token->quoting_info, token->quoting_info);
		}
		else if (token->type == TOK_WORD || token->type == TOK_SQUOTE_STR
			|| token->type == TOK_DQUOTE_STR)
		{
			if (!word_token)
				word_token = new_token("", TOK_WORD, true);
			word_token->str = ft_strjoin(word_token->str, token->str);
			word_token->quoting_info = ft_strjoin(word_token->quoting_info, token->quoting_info);
			lpush(new_tokens, as_token(word_token));
			word_token = NULL;
		}
		else
		{
			if (word_token)
			{
				lpush(new_tokens, as_token(word_token));
				word_token = NULL;
			}
			lpush(new_tokens, as_token(token));
		}
	}
	tokens->first = new_tokens->first;
	tokens->last = new_tokens->last;
	tokens->len = new_tokens->len;
}
