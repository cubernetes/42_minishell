/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_copy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:52 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:32:46 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

t_ddeque	*ddeque_shallow_copy(t_ddeque *ddeque)
{
	t_ddeque		*new_ddeque;
	t_ddeque_node	*orig_head;
	t_ddeque_node	*head;

	new_ddeque = ddeque_init();
	if (!ddeque->head)
		return (new_ddeque);
	orig_head = ddeque->head;
	head = orig_head;
	ddeque_push_value_bottom(new_ddeque, head->data);
	head = head->next;
	while (head != orig_head)
	{
		ddeque_push_value_bottom(new_ddeque, head->data);
		head = head->next;
	}
	return (new_ddeque);
}
