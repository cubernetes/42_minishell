#include "../../include/minishell.h"
#include "../../libft/libft.h"

static size_t	expand_vars(t_token *token, char *var)
{
	char	*orig_var;
	size_t	len;
	char	*expanded_var;

	if (var[0] == '$' && ft_isalpha(var[1]))
	{
		orig_var = ++var;
		len = 0;
		while (ft_isalpha(*var) || ft_isdigit(*var) || *var == '_')
		{
			++var;
			++len;
		}
		expanded_var = env_lookup(ft_strndup(orig_var, len));
		token->str = ft_strjoin(token->str, expanded_var);
		return (len + 1);
	}
	token->str = ft_strjoin(token->str, ft_strndup(var, 1));
	return (1);
}

static void	*new_word_token(char *str)
{
	return (new_token(str, TOK_WORD, TRUE));
}

/* TODO: Not required: use full IFS instead of just the first char */
static void	expand_word(t_deque *new_tokens, t_deque_node *head)
{
	t_token	*token;
	char	*token_str;
	t_deque	*new_toks;

	token = head->as_token;
	if (token->type != TOK_WORD)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	new_toks = string_list_to_deque(ft_split(token->str, IFS[0]), new_word_token);
	deque_extend_right(new_tokens, new_toks);
	if (!token->is_last_subtoken)
		new_tokens->head->prev->as_token->is_last_subtoken = FALSE;
}

static void	expand_dquote_str(t_deque *new_tokens, t_deque_node *head)
{
	t_token	*token;
	char	*token_str;

	token = head->as_token;
	if (token->type != TOK_DQUOTE_STR)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	deque_push_ptr_right(new_tokens, token);
}

static void	expand(t_deque *new_tokens, t_deque_node *head)
{
	if (head->as_token->type == TOK_WORD)
		expand_word(new_tokens, head);
	else if (head->as_token->type == TOK_DQUOTE_STR)
		expand_dquote_str(new_tokens, head);
	else
		deque_push_ptr_right(new_tokens, head->as_ptr);
}

void	expand_env_vars(t_deque *tokens)
{
	t_deque			*new_tokens;
	t_di			*di;

	new_tokens = deque_init();
	di = di_begin(tokens);
	while (di_next(di))
		expand(new_tokens, di_get(di));
	tokens->head = new_tokens->head;
	tokens->size = new_tokens->size;
}

void	join_tokens(t_deque *tokens)
{
	t_deque_node	*head;
	t_deque			*new_tokens;
	t_token			*token;
	t_token			*word_token;

	new_tokens = deque_init();
	head = tokens->head;
	if (!head)
		return ;
	token = head->as_token;
	word_token = NULL;
	if (token->is_last_subtoken)
		deque_push_ptr_right(new_tokens, token);
	else
		word_token = new_token(token->str, TOK_WORD, TRUE);
	while (head->next != tokens->head)
	{
		head = head->next;
		token = head->as_token;
		if (!token->is_last_subtoken)
		{
			if (!word_token)
				word_token = new_token("", TOK_WORD, TRUE);
			word_token->str = ft_strjoin(word_token->str, token->str);
		}
		else if (token->type == TOK_WORD || token->type == TOK_SQUOTE_STR
			|| token->type == TOK_DQUOTE_STR)
		{
			if (!word_token)
				word_token = new_token("", TOK_WORD, TRUE);
			word_token->str = ft_strjoin(word_token->str, token->str);
			deque_push_ptr_right(new_tokens, word_token);
			word_token = NULL;
		}
		else
		{
			if (word_token)
			{
				deque_push_ptr_right(new_tokens, word_token);
				word_token = NULL;
			}
			deque_push_ptr_right(new_tokens, token);
		}
	}
	tokens->head = new_tokens->head;
	tokens->size = new_tokens->size;
}
