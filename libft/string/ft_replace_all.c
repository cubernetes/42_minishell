/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_all.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:03:21 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:34:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_replace.h"

static void	_maybe_replace(t_args args[static 1])
{
	if (!ft_strncmp(*args->str + *args->str_idx, *args->pattern,
			*args->pattern_len))
	{
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
}

/** Return a new string where the all occurences of a fixed-string pattern
 *  in a reference string are replaced by another string.
 *
 *  @param str: the reference string in which to look for `pattern'
 *  @param pattern: the fixed-string pattern to be searched for in `str'
 *  @param replacement: the string that replaces all occurences of `str'
 *  @returns: a newly allocated string where all occurences of
 *            `pattern' are replaced with `replacement'
 */
char	*ft_replace_all(const char str[static 1], const char pattern[static 1],
		const char replacement[static 1])
{
	size_t	pattern_len;
	size_t	replacement_len;
	size_t	str_idx;
	size_t	new_str_idx;
	char	*new_str;

	pattern_len = ft_strlen(pattern);
	replacement_len = ft_strlen(replacement);
	new_str = ft_malloc(sizeof(*new_str) * (1 + ft_strlen(str)
				+ (replacement_len - pattern_len) * ft_count_all(str,
					pattern)));
	str_idx = 0;
	new_str_idx = 0;
	while (str[str_idx])
		_maybe_replace(&(t_args){&str, &str_idx, &replacement_len, &replacement,
			&pattern_len, &new_str_idx, &pattern, &new_str});
	if (!pattern_len)
	{
		ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
		new_str_idx += replacement_len;
	}
	new_str[new_str_idx] = 0;
	return (new_str);
}
