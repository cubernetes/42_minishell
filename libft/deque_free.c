/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:19 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 01:31:45 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int	deque_free(t_deque *deque)
{
	t_deque_node	*head;
	t_deque_node	*tail;

	if (!deque)
		return (1);
	head = deque->head;
	if (!head)
	{
		(free(deque), deque = NULL);
		return (1);
	}
	tail = head->prev;
	while (head != tail)
	{
		head = head->next;
		(free(head->prev), head->prev = NULL);
	}
	(free(head), head = NULL);
	(free(deque), deque = NULL);
	return (1);
}
