/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_pop_right.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 01:44:47 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 17:52:08 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_ddeque_node	*ddeque_pop_right(t_ddeque *ddeque)
{
	t_ddeque_node	*head;
	t_ddeque_node	*prev;
	t_ddeque_node	*prev_prev;

	head = ddeque->head;
	if (!head)
		return (NULL);
	prev = ddeque->head->prev;
	prev_prev = prev->prev;
	if (head == head->next)
		ddeque->head = NULL;
	else
	{
		ddeque->head->prev = prev_prev;
		prev_prev->next = ddeque->head;
	}
	prev->next = NULL;
	prev->prev = NULL;
	ddeque->size -= 1;
	return (prev);
}
