#include "minishell.h"
#include "libft.h"

#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>

static bool	glob_match(const char *pattern, const char *quoting_info,
	const char *str)
{
	if (*pattern == '\0')
		return (*str == '\0');
	else if (*pattern == '*' && *quoting_info == '0')
	{
		if (*str != '\0' && glob_match(pattern, quoting_info, str + 1))
			return (true);
		return (glob_match(pattern + 1, quoting_info + 1, str));
	}
	else if ((*pattern == '?' && *quoting_info == '0') || *str == *pattern)
		return (glob_match(pattern + 1, quoting_info + 1, str + 1));
	return (false);
}

static bool	ft_strcmp2(t_data str1, t_data str2)
{
	return ((bool) !ft_strcmp(str1.as_str, str2.as_str));
}

/* mirroring bash's behaviour of NOT globbing when it fails to open a dir */
/* TODO: expand .* to hidden files */
t_list	*glob_token(t_token *token)
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
			&& glob_match(token->str, token->quoting_info, dp->d_name))
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

/* TODO: apply DRY */
/* TODO: apply DRY in the deque/deque functions */
void	glob_tokens(t_list *tokens)
{
	t_list		*new_tokens;

	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
	{
		if (tokens->current->as_token->type == TOK_WORD)
			lextend(new_tokens, glob_token(tokens->current->as_token));
		else
			lpush(new_tokens, as_data(tokens->current));
	}
	tokens->first = new_tokens->first;
	tokens->last = new_tokens->last;
	tokens->len = new_tokens->len;
}

t_list	*glob_tokens_2(t_list *tokens)
{
	t_list		*new_tokens;

	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
	{
		if (tokens->current->as_token->type == TOK_WORD)
			lextend(new_tokens, glob_token(tokens->current->as_token));
		else
			lpush(new_tokens, as_data(tokens->current));
	}
	return (new_tokens);
}
