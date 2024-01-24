/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_pop_bottom.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 01:44:47 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 17:52:02 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_deque_node	*deque_pop_bottom(t_deque *deque)
{
	t_deque_node	*head;
	t_deque_node	*prev;
	t_deque_node	*prev_prev;

	head = deque->head;
	if (!head)
		return (NULL);
	prev = deque->head->prev;
	prev_prev = prev->prev;
	if (head == head->next)
		deque->head = NULL;
	else
	{
		deque->head->prev = prev_prev;
		prev_prev->next = deque->head;
	}
	prev->next = NULL;
	prev->prev = NULL;
	deque->size -= 1;
	return (prev);
}
