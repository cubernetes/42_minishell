/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:19 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/26 23:31:57 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ddeque_free(t_ddeque *ddeque, void (free_data)(void *))
{
	t_ddeque_node	*head;
	t_ddeque_node	*tail;

	if (!ddeque)
		return ;
	head = ddeque->head;
	if (!head)
	{
		(free(ddeque), ddeque = NULL);
		return ;
	}
	tail = head->prev;
	while (head != tail)
	{
		head = head->next;
		free_data(head->prev->data);
		(free(head->prev), head->prev = NULL);
	}
	free_data(head->data);
	(free(head), head = NULL);
	(free(ddeque), ddeque = NULL);
}
