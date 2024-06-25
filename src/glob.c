#include "minishell.h"
#include "libft.h"

#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>

static bool	glob_match(const char *pattern, const char *quoting_ctx,
	const char *str)
{
	if (*pattern == '\0')
		return (*str == '\0');
	else if (*pattern == '*' && *quoting_ctx == '0')
	{
		if (*str != '\0' && glob_match(pattern, quoting_ctx, str + 1))
			return (true);
		return (glob_match(pattern + 1, quoting_ctx + 1, str));
	}
	else if ((*pattern == '?' && *quoting_ctx == '0') || *str == *pattern)
	{
		if (*str == '\0')
			return (false);
		return (glob_match(pattern + 1, quoting_ctx + 1, str + 1));
	}
	return (false);
}

static bool	ft_strcmp2(t_data str1, t_data str2)
{
	return ((bool) !ft_strcmp(str1.as_str, str2.as_str));
}

static t_list	*dotglob(t_list *tokens, t_token *token)
{
	t_list	*new_tokens;
	bool	do_dotglob;

	do_dotglob = false;
	if (token->str[0] == '.')
		do_dotglob = true;
	new_tokens = lnew();
	liter(tokens);
	while (lnext(tokens))
	{
		if (tokens->current->as_token->str[0] == '.')
		{
			if (do_dotglob)
				lpush(new_tokens, as_data(tokens->current));
		}
		else
			lpush(new_tokens, as_data(tokens->current));
	}
	return (new_tokens);
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
		if (glob_match(token->str, token->quoting_ctx, dp->d_name) && ft_strcmp(dp->d_name, ".") && ft_strcmp(dp->d_name, ".."))
			lpush(tokens,
				as_token(new_token(ft_strdup(dp->d_name), TOK_WORD, true)));
		dp = readdir(dirp);
	}
	closedir(dirp);
	if (tokens->len == 0)
		lpush(tokens, as_token(token));
	lsort(tokens, ft_strcmp2);
	tokens = liter(dotglob(tokens, token));
	while (lnext(tokens))
		tokens->current->as_token->num_tokens_after_split = (int)tokens->len + token->num_tokens_after_split - 1;
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
