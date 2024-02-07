/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list_to_ddeque.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/07 07:53:36 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_ddeque	*array_list_to_ddeque(char **array_list, void *(*new_node)(void *))
{
	t_ddeque	*ddeque;

	ddeque = ddeque_init();
	while (*array_list)
	{
		ddeque_push_value_bottom(ddeque, new_node(*array_list));
		++array_list;
	}
	return (ddeque);
}
