/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_pop_top.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:41:09 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 17:52:14 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_ddeque_node	*ddeque_pop_top(t_ddeque *ddeque)
{
	t_ddeque_node	*prev;
	t_ddeque_node	*head;
	t_ddeque_node	*next;

	head = ddeque->head;
	if (!head)
		return (NULL);
	prev = head->prev;
	next = head->next;
	if (head == head->next)
		ddeque->head = NULL;
	else
	{
		ddeque->head = next;
		next->prev = prev;
		prev->next = next;
	}
	head->next = NULL;
	head->prev = NULL;
	ddeque->size -= 1;
	return (head);
}
