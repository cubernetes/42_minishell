#include "minishell.h"
#include "libft.h"

#include <stdlib.h>

char	*var_lookup(char *key)
{
	char	*value;

	value = getenv(key);
	if (value != NULL)
		return (value);
	return (get_var(key));
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
		expanded_var = get_var("?");
		token->str = ft_strjoin(token->str, ft_strdup(expanded_var));
		return (2);
	}
	token->str = ft_strjoin(token->str, ft_strndup(var, 1));
	return (1);
}

/* TODO: Not required: use full IFS instead of just the first char */
static void	expand_word(t_list *new_tokens, t_list_node *first)
{
	t_token	*token;
	char	*token_str;
	t_list	*new_toks;

	token = first->as_token;
	if (token->type != TOK_WORD)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	new_toks = lsplit(token->str, ft_strndup(IFS, 1));
	lextend(new_tokens, new_toks);
	if (!token->is_last_subtoken)
		new_tokens->first->prev->as_token->is_last_subtoken = false;
}

static void	expand_dquote_str(t_list *new_tokens, t_list_node *first)
{
	t_token	*token;
	char	*token_str;

	token = first->as_token;
	if (token->type != TOK_DQUOTE_STR)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	lpush(new_tokens, as_token(token));
}

static void	expand(t_list *new_tokens, t_list_node *first)
{
	if (first->as_token->type == TOK_WORD)
		expand_word(new_tokens, first);
	else if (first->as_token->type == TOK_DQUOTE_STR)
		expand_dquote_str(new_tokens, first);
	else
		lpush(new_tokens, as_token(first->as_token));
}

void	expand_env_vars(t_list *tokens)
{
	t_list	*new_tokens;

	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
		expand(new_tokens, tokens->current);
	tokens->first = new_tokens->first;
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
		}
		else if (token->type == TOK_WORD || token->type == TOK_SQUOTE_STR
			|| token->type == TOK_DQUOTE_STR)
		{
			if (!word_token)
				word_token = new_token("", TOK_WORD, true);
			word_token->str = ft_strjoin(word_token->str, token->str);
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
	tokens->len = new_tokens->len;
}
