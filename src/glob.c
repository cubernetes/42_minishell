#include "minishell.h"

#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>

static bool	glob_match(const char *pattern, const char *str)
{
	if (*pattern == '\0')
		return (*str == '\0');
	else if (*pattern == '*')
	{
		if (*str != '\0' && glob_match(pattern, str + 1))
			return (true);
		return (glob_match(pattern + 1, str));
	}
	else if (*pattern == '?' || *str == *pattern)
		return (glob_match(pattern + 1, str + 1));
	return (false);
}

static int	ft_strcmp2(const char *str1, const char *str2)
{
	return (!ft_strcmp(str1, str2));
}

static t_deque	*glob_token(t_token *token)
{
	DIR				*dirp;
	struct dirent	*dp;
	t_deque			*tokens;

	tokens = deque_init();
	dirp = opendir(".");
	if (!dirp)
		return (deque_push_ptr_right(tokens, token), tokens);
	dp = readdir(dirp);
	while (dp != NULL)
	{
		if (ft_strncmp(dp->d_name, ".", 1)
			&& glob_match(token->str, dp->d_name))
			deque_push_ptr_right(tokens,
				new_token(ft_strdup(dp->d_name), TOK_WORD, true));
		dp = readdir(dirp);
	}
	closedir(dirp);
	if (tokens->size == 0)
		deque_push_ptr_right(tokens, token);
	deque_sort(tokens, (int (*)(void *, void *))ft_strcmp2);
	return (tokens);
}

/* mirroring bash's behaviour of NOT globbing when it fails to open a dir */
/* TODO: apply DRY */
/* TODO: apply DRY in the deque/deque functions */
/* TODO: expand .* to hidden files */
void	glob_tokens(t_deque *tokens)
{
	t_deque			*new_tokens;
	t_deque_node	*head;

	new_tokens = deque_init();
	head = tokens->head;
	if (head)
	{
		if (head->as_token->type == TOK_WORD)
			deque_extend_right(new_tokens, glob_token(head->as_token));
		else
			deque_push_ptr_right(new_tokens, head->as_ptr);
		while (head->next != tokens->head)
		{
			head = head->next;
			if (head->as_token->type == TOK_WORD)
				deque_extend_right(new_tokens, glob_token(head->as_token));
			else
				deque_push_ptr_right(new_tokens, head->as_ptr);
		}
	}
	tokens->head = new_tokens->head;
	tokens->size = new_tokens->size;
}
