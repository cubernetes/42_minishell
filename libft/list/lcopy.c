/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcopy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:43 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:04:43 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Return a shallow copy of a list
 *
 *  @param list: the list to create the shallow copy of
 *  @returns: a shallow copy of `list'
 */
t_list	*lcopy(t_list *list)
{
	t_list	*new_list;

	new_list = lnew();
	liter(list);
	while (lnext(list))
		lpush(new_list, as_data(list->current));
	return (new_list);
}
