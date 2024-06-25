#include "minishell.h"
#include "libft.h"

void	join_tokens(t_list *tokens)
{
	t_list_node	*first;
	t_list		*new_tokens;
	t_list		*cleaned_tokens;
	t_token		*token;
	t_token		*word_token;

	new_tokens = lnew();
	first = tokens->first;
	if (!first)
		return ;
	token = first->as_token;
	word_token = NULL;
	if (token->is_last_token)
		lpush(new_tokens, as_token(token));
	else
		word_token = new_token(token->str, TOK_WORD, true);
	while (first->next != tokens->first)
	{
		first = first->next;
		token = first->as_token;
		if (!token->is_last_token)
		{
			if (!word_token)
			{
				word_token = new_token("", TOK_WORD, true);
				word_token->num_tokens_after_split = 0;
			}
			word_token->num_tokens_after_split = ft_max(word_token->num_tokens_after_split, token->num_tokens_after_split);
			word_token->str = ft_strjoin(word_token->str, token->str);
			word_token->quoting_ctx = ft_strjoin(word_token->quoting_ctx, token->quoting_ctx);
			word_token->expansion_ctx = ft_strjoin(word_token->expansion_ctx, token->expansion_ctx);
		}
		else if (token->type == TOK_WORD || token->type == TOK_SQUOTE_STR
			|| token->type == TOK_DQUOTE_STR)
		{
			if (!word_token)
			{
				word_token = new_token("", TOK_WORD, true);
				word_token->num_tokens_after_split = 0;
			}
			word_token->num_tokens_after_split = ft_max(word_token->num_tokens_after_split, token->num_tokens_after_split);
			word_token->str = ft_strjoin(word_token->str, token->str);
			word_token->quoting_ctx = ft_strjoin(word_token->quoting_ctx, token->quoting_ctx);
			word_token->expansion_ctx = ft_strjoin(word_token->expansion_ctx, token->expansion_ctx);
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
	cleaned_tokens = lnew();
	liter(new_tokens);
	while (lnext(new_tokens))
		if (new_tokens->current->as_token->str[0] != '\0' || new_tokens->current->as_token->type == TOK_EOL)
			lpush(cleaned_tokens, as_data(new_tokens->current));
	tokens->first = cleaned_tokens->first;
	tokens->last = cleaned_tokens->last;
	tokens->len = cleaned_tokens->len;
}
