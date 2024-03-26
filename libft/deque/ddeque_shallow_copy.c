/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_shallow_copy.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:52 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:47:21 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

t_ddeque	*ddeque_shallow_copy(t_ddeque *ddeque)
{
	t_ddeque		*new_ddeque;
	t_ddeque_node	*head;

	new_ddeque = ddeque_init();
	if (!ddeque->head)
		return (new_ddeque);
	head = ddeque->head;
	ddeque_push_value_right(new_ddeque, head->data);
	while (head->next != ddeque->head)
	{
		head = head->next;
		ddeque_push_value_right(new_ddeque, head->data);
	}
	return (new_ddeque);
}
