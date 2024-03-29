/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_equal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:24:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/27 17:59:23 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* cmp must return 0 if comparison fails */
t_bool	deque_equal(t_deque *deque_a, t_deque *deque_b,
		t_bool (cmp)(void *ptr1, void *ptr2))
{
	t_deque_node	*head_a;
	t_deque_node	*head_b;

	if (deque_a->size != deque_b->size)
		return (FALSE);
	if (deque_a->size == 0)
		return (TRUE);
	head_a = deque_a->head->next;
	head_b = deque_b->head->next;
	while (head_a != deque_a->head && head_b != deque_b->head)
	{
		if (!cmp(head_a->as_ptr, head_b->as_ptr))
			return (FALSE);
		head_a = head_a->next;
		head_b = head_b->next;
	}
	if (head_a == deque_a->head && head_b == deque_b->head)
		return (TRUE);
	return (FALSE);
}
