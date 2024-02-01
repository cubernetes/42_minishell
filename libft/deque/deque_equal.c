/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_equal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:24:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 16:15:37 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	deque_equal(t_deque *deque_a, t_deque *deque_b)
{
	t_deque_node	*head_a;
	t_deque_node	*head_b;

	if (deque_a->size != deque_b->size)
		return (0);
	if (deque_a->size == 0)
		return (1);
	head_a = deque_a->head->next;
	head_b = deque_b->head->next;
	while (head_a != deque_a->head && head_b != deque_b->head)
	{
		if (head_a->data != head_b->data)
			return (0);
		head_a = head_a->next;
		head_b = head_b->next;
	}
	if (head_a == deque_a->head && head_b == deque_b->head)
		return (1);
	return (0);
}
