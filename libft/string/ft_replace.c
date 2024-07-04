/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:03:07 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:48:51 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_replace.h"
#include "libft.h"

static bool	_maybe_replace_once(t_args *args)
{
	static bool	replaced;

	if (args == NULL)
		return (replaced);
	if ((t_args *)args == (t_args *)1)
		return (replaced = false);
	if (!ft_strncmp(*args->str + *args->str_idx, *args->pattern,
			*args->pattern_len) && !replaced)
	{
		replaced = true;
		ft_strlcpy(*args->new_str + *args->new_str_idx, *args->replacement,
			*args->replacement_len + 1);
		*args->str_idx += *args->pattern_len;
		*args->new_str_idx += *args->replacement_len;
		if (!*args->pattern_len)
			(*args->new_str)[(*args->new_str_idx)++]
				= (*args->str)[(*args->str_idx)++];
	}
	else
		(*args->new_str)[(*args->new_str_idx)++]
			= (*args->str)[(*args->str_idx)++];
	return (replaced);
}

/** Return a new string where the left-most occurence of a fixed-string pattern
 *  in a reference string is replaced by another string.
 *
 *  @param str: the reference string in which to look for `pattern'
 *  @param pattern: the fixed-string pattern to be searched for in `str'
 *  @param replacement: the string that replaces the first occurence of `str'
 *  @returns: a newly allocated string where the left-most occurence of
 *            `pattern' is replaced with `replacement'
 */
char	*ft_replace(const char str[static 1], const char pattern[static 1],
		const char replacement[static 1])
{
	size_t	pattern_len;
	size_t	replacement_len;
	size_t	str_idx;
	size_t	new_str_idx;
	char	*new_str;

	pattern_len = ft_strlen(pattern);
	replacement_len = ft_strlen(replacement);
	if (ft_count_all(str, pattern) == 0)
		return (ft_strdup(str));
	new_str = ft_malloc(sizeof(*new_str) * (1 + ft_strlen(str)
				+ (replacement_len - pattern_len)));
	str_idx = 0;
	new_str_idx = 0;
	_maybe_replace_once((t_args *)1);
	while (str[str_idx])
		(void)_maybe_replace_once(&(t_args){&str, &str_idx, &replacement_len,
			&replacement, &pattern_len, &new_str_idx, &pattern, &new_str});
	if (!pattern_len && !_maybe_replace_once(NULL))
	{
		ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
		new_str_idx += replacement_len;
	}
	new_str[new_str_idx] = 0;
	return (new_str);
}
