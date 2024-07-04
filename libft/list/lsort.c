/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lsort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:58:17 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:58:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

/** Sort a list in-place and return the same list
 *
 *  @param list: the list to sort
 *  @param cmp: a comparison function that takes two data unions and returns
 *              a bool
 *  @returns: `list'
 */
t_list	*lsort(t_list *list, bool (cmp)(t_data data1, t_data data2))
{
	size_t	i;
	size_t	j;

	i = (size_t)(-1);
	while (++i < list->len - 1)
	{
		j = (size_t)(-1);
		while (++j < list->len - 1 - i)
		{
			if (cmp(as_data(list->first), as_data(list->first->next)))
				lswap(list);
			lrotate(list, 1);
		}
		while (j++ < list->len)
			lrotate(list, 1);
	}
	return (list);
}
