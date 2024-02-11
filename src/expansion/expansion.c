/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 06:13:50 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/11 12:01:46 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	*new_word_token(void *data)
{
	return (new_token((char *)data, TOK_WORD, TRUE));
}

static void	expand_word(t_ddeque *new_tokens, t_ddeque_node *head)
{
	t_token		*token;
	char		*token_str;
	t_ddeque	*new_toks;

	token = (t_token *)head->data;
	if (token->type != TOK_WORD)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	new_toks = array_list_to_ddeque(ft_split(token->str, IFS), new_word_token);
	ddeque_extend(new_tokens, new_toks);
	if (!token->is_last_subtoken)
		((t_token *)new_tokens->head->prev->data)->is_last_subtoken = FALSE;
}

char	**set_environ(char **envp)
{
	static char	**_envp = NULL;

	if (envp)
		_envp = envp;
	return (_envp);
}

char	**get_environ(void)
{
	return (set_environ(NULL));
}

char	*env_lookup(char *var)
{
	char	**envp;
	size_t	len;

	envp = get_environ();
	while (envp && envp[0])
	{
		len = ft_strlen(var);
		if (!ft_strncmp(envp[0], var, len)
			&& envp[0][len] == '=')
			return (envp[0] + len + 1);
		++envp;
	}
	return ("");
}

static void	expand_dquote_str(t_ddeque *new_tokens, t_ddeque_node *head)
{
	t_token	*token;
	char	*token_str;

	token = (t_token *)head->data;
	if (token->type != TOK_DQUOTE_STR)
		return ;
	token_str = token->str;
	token->str = "";
	while (*token_str)
		token_str += expand_vars(token, token_str);
	ddeque_push_value_bottom(new_tokens, token);
}

static void	expand(t_ddeque *new_tokens, t_ddeque_node *head)
{
	if (((t_token *)head->data)->type == TOK_WORD)
		expand_word(new_tokens, head);
	else if (((t_token *)head->data)->type == TOK_DQUOTE_STR)
		expand_dquote_str(new_tokens, head);
	else
		ddeque_push_value_bottom(new_tokens, head->data);
}

void	expand_env_vars(t_ddeque *tokens)
{
	t_ddeque_node	*orig_head;
	t_ddeque_node	*head;
	t_ddeque		*new_tokens;

	new_tokens = ddeque_init();
	head = tokens->head;
	orig_head = head;
	if (!head)
		return ;
	expand(new_tokens, head);
	while (head->next != orig_head)
	{
		head = head->next;
		expand(new_tokens, head);
	}
	tokens->head = new_tokens->head;
}

void	join_tokens(t_ddeque *tokens)
{
	t_ddeque_node	*orig_head;
	t_ddeque_node	*head;
	t_ddeque		*new_tokens;
	t_token			*token;
	t_token			*word_token;

	new_tokens = ddeque_init();
	head = tokens->head;
	orig_head = head;
	if (!head)
		return ;
	token = (t_token *)head->data;
	word_token = NULL;
	if (token->is_last_subtoken)
		ddeque_push_value_bottom(new_tokens, token);
	else
		word_token = new_token(token->str, TOK_WORD, TRUE);
	while (head->next != orig_head)
	{
		head = head->next;
		token = (t_token *)head->data;
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
			ddeque_push_value_bottom(new_tokens, word_token);
			word_token = NULL;
		}
		else
		{
			if (word_token)
			{
				ddeque_push_value_bottom(new_tokens, word_token);
				word_token = NULL;
			}
			ddeque_push_value_bottom(new_tokens, token);
		}
	}
	tokens->head = new_tokens->head;
}
