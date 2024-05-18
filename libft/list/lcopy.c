/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcopy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:52 by tischmid          #+#    #+#             */
/*   Updated: 2024/05/18 17:50:06 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*lcopy(t_list *list)
{
	t_list	*new_list;

	new_list = lnew();
	liter(list);
	while (lnext(list))
		lpush(new_list, as_data(list->current));
	return (new_list);
}
