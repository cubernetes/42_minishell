#include "minishell.h"
#include "libft.h"

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

static bool	ft_strcmp2(t_data str1, t_data str2)
{
	return ((bool)!ft_strcmp(str1.as_str, str2.as_str));
}

static t_list	*glob_token(t_token *token)
{
	DIR				*dirp;
	struct dirent	*dp;
	t_list			*tokens;

	tokens = lnew();
	dirp = opendir(".");
	if (!dirp)
		return (lpush(tokens, as_token(token)), tokens);
	dp = readdir(dirp);
	while (dp != NULL)
	{
		if (ft_strncmp(dp->d_name, ".", 1)
			&& glob_match(token->str, dp->d_name))
			lpush(tokens,
				as_token(new_token(ft_strdup(dp->d_name), TOK_WORD, true)));
		dp = readdir(dirp);
	}
	closedir(dirp);
	if (tokens->len == 0)
		lpush(tokens, as_token(token));
	lsort(tokens, ft_strcmp2);
	return (tokens);
}

/* mirroring bash's behaviour of NOT globbing when it fails to open a dir */
/* TODO: apply DRY */
/* TODO: apply DRY in the deque/deque functions */
/* TODO: expand .* to hidden files */
void	glob_tokens(t_list *tokens)
{
	t_list		*new_tokens;
	t_list_node	*first;

	new_tokens = lnew();
	first = tokens->first;
	if (first)
	{
		if (first->as_token->type == TOK_WORD)
			lextend(new_tokens, glob_token(first->as_token));
		else
			lpush(new_tokens, as_token(first->as_token));
		while (first->next != tokens->first)
		{
			first = first->next;
			if (first->as_token->type == TOK_WORD)
				lextend(new_tokens, glob_token(first->as_token));
			else
				lpush(new_tokens, as_token(first->as_token));
		}
	}
	tokens->first = new_tokens->first;
	tokens->last = new_tokens->last;
	tokens->len = new_tokens->len;
}
