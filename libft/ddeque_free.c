/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:19 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 03:41:35 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_bool	ddeque_free(t_ddeque *ddeque, t_bool (free_data)(void *))
{
	t_ddeque_node	*head;
	t_ddeque_node	*tail;

	if (!ddeque)
		return (TRUE);
	head = ddeque->head;
	if (!head)
	{
		(free(ddeque), ddeque = NULL);
		return (TRUE);
	}
	tail = head->prev;
	while (head != tail)
	{
		head = head->next;
		(void)free_data(head->prev->data);
		(free(head->prev), head->prev = NULL);
	}
	free_data(head->data);
	(free(head), head = NULL);
	(free(ddeque), ddeque = NULL);
	return (TRUE);
}
