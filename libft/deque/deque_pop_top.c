/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_pop_top.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:41:09 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 17:51:44 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_deque_node	*deque_pop_top(t_deque *deque)
{
	t_deque_node	*prev;
	t_deque_node	*head;
	t_deque_node	*next;

	head = deque->head;
	if (!head)
		return (NULL);
	prev = head->prev;
	next = head->next;
	if (head == head->next)
		deque->head = NULL;
	else
	{
		deque->head = next;
		next->prev = prev;
		prev->next = next;
	}
	head->next = NULL;
	head->prev = NULL;
	deque->size -= 1;
	return (head);
}
