/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_all.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 20:06:06 by tosuman           #+#    #+#             */
/*   Updated: 2024/07/04 20:06:06 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Count how many instances of one string appear in another string.
 *
 *  @param str: the string to search in
 *  @param pattern: the fixed string to count the occurences of in `str'
 *  @returns: the number of fixed string occurences of `pattern` in `str'
 */
size_t	ft_count_all(const char str[static 1], const char pattern[static 1])
{
	size_t	pattern_len;
	size_t	pattern_count;

	pattern_len = ft_strlen(pattern);
	pattern_count = 0;
	while (*str)
	{
		if (!ft_strncmp(str, pattern, pattern_len))
		{
			pattern_count += 1;
			str += pattern_len;
			if (!pattern_len)
				++str;
		}
		else
			++str;
	}
	if (!pattern_len)
		pattern_count += 1;
	return (pattern_count);
}
