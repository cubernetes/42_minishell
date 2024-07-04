/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lsplit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:54:33 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:55:11 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Split a string on a string delimiter and return a list.
 *
 *  @param str: the string the split
 *  @param delim: the string delimiter to split on (must be at least 1 char)
 *  @returns: a new list containing the elements of the split operation
 */
t_list	*lsplit(const char str[static 1], const char delim[static 2])
{
	t_list	*list;
	size_t	token_start;
	size_t	token_end;
	size_t	delim_len;

	delim_len = ft_strlen(delim);
	list = lnew();
	if (delim_len == 0)
		return (list);
	token_start = 0;
	token_end = 0;
	while (str[token_end])
	{
		if (!ft_strncmp(str + token_end, delim, delim_len))
		{
			(void)lpush(list, as_str(ft_strndup(str + token_start,
						(size_t)(token_end - token_start))));
			token_end += delim_len;
			token_start = token_end;
			--token_end;
		}
		++token_end;
	}
	return ((void)lpush(list, as_str(ft_strndup(str + token_start,
					(size_t)(token_end - token_start)))), list);
}
