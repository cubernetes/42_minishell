#include "libft.h"

/** Return a new string where the left-most occurence of a fixed-string pattern
 *  in a reference string is replaced by another string.
 *
 *  @param str: the reference string in which to look for `pattern'
 *  @param pattern: the fixed-string pattern to be searched for in `str'
 *  @param replacement: the string that replaces the first occurence of `str'
 *  @returns: a newly allocated string where the left-most occurence of
 *            `pattern' is replaced with `replacement'
 */
char	*ft_replace(
	const char str[static 1],
	const char pattern[static 1],
	const char replacement[static 1])
{
	size_t	pattern_len;
	size_t	replacement_len;
	size_t	str_idx;
	size_t	new_str_idx;
	bool	replaced;
	char	*new_str;

	pattern_len = ft_strlen(pattern);
	replacement_len = ft_strlen(replacement);
	if (ft_count_all(str, pattern) == 0)
		return (ft_strdup(str));
	new_str = ft_malloc(sizeof(*new_str) * (1 + ft_strlen(str)
				+ (replacement_len - pattern_len)));
	str_idx = 0;
	new_str_idx = 0;
	replaced = false;
	while (str[str_idx])
	{
		if (!ft_strncmp(str + str_idx, pattern, pattern_len) && !replaced)
		{
			replaced = true;
			ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
			str_idx += pattern_len;
			new_str_idx += replacement_len;
			if (!pattern_len)
				new_str[new_str_idx++] = str[str_idx++];
		}
		else
			new_str[new_str_idx++] = str[str_idx++];
	}
	if (!pattern_len && !replaced)
	{
		ft_strlcpy(new_str + new_str_idx, replacement, replacement_len + 1);
		new_str_idx += replacement_len;
	}
	new_str[new_str_idx] = 0;
	return (new_str);
}
