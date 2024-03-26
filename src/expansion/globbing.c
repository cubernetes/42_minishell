#include "../../include/minishell.h"
#include <dirent.h>
#include <stdio.h>

static t_bool	glob_match(const char *pattern, const char *str)
{
	if (*pattern == '\0')
		return (*str == '\0');
	else if (*pattern == '*')
	{
		if (*str != '\0' && glob_match(pattern, str + 1))
			return (TRUE);
		return (glob_match(pattern + 1, str));
	}
	else if (*pattern == '?' || *str == *pattern)
		return (glob_match(pattern + 1, str + 1));
	return (FALSE);
}

static t_ddeque	*glob_token(t_token *token)
{
	DIR				*dirp;
	struct dirent	*dp;
	t_ddeque		*tokens;

	tokens = ddeque_init();
	dirp = opendir(".");
	if (!dirp)
		return (ddeque_push_value_right(tokens, token), tokens);
	dp = readdir(dirp);
	while (dp != NULL)
	{
		if (ft_strncmp(dp->d_name, ".", 1)
			&& glob_match(token->str, dp->d_name))
			ddeque_push_value_right(tokens,
				new_token(ft_strdup(dp->d_name), TOK_WORD, TRUE));
		dp = readdir(dirp);
	}
	closedir(dirp);
	if (tokens->size == 0)
		ddeque_push_value_right(tokens, token);
	return (tokens);
}

/* mirroring bash's behaviour of NOT globbing when it fails to open a dir */
/* TODO: apply DRY */
/* TODO: apply DRY in the deque/ddeque functions */
void	glob_tokens(t_ddeque *tokens)
{
	t_ddeque		*new_tokens;
	t_ddeque_node	*head;

	new_tokens = ddeque_init();
	head = tokens->head;
	if (head)
	{
		if (((t_token *)head->data)->type == TOK_WORD)
			ddeque_extend_right(new_tokens, glob_token(head->data));
		else
			ddeque_push_value_right(new_tokens, head->data);
		while (head->next != tokens->head)
		{
			head = head->next;
			if (((t_token *)head->data)->type == TOK_WORD)
				ddeque_extend_right(new_tokens, glob_token(head->data));
			else
				ddeque_push_value_right(new_tokens, head->data);
		}
	}
	tokens->head = new_tokens->head;
	tokens->size = new_tokens->size;
}
