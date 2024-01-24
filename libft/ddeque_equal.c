/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_equal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:24:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:44:20 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* cmp must return 0 if values are not equal */
int	ddeque_equal(t_ddeque *ddeque_a, t_ddeque *ddeque_b,
		int (cmp)(void *, void *))
{
	t_ddeque_node	*head_a;
	t_ddeque_node	*head_b;

	if (ddeque_a->size != ddeque_b->size)
		return (0);
	if (ddeque_a->size == 0)
		return (1);
	head_a = ddeque_a->head->next;
	head_b = ddeque_b->head->next;
	while (head_a != ddeque_a->head && head_b != ddeque_b->head)
	{
		if (!cmp(head_a->data, head_b->data))
			return (0);
		head_a = head_a->next;
		head_b = head_b->next;
	}
	if (head_a == ddeque_a->head && head_b == ddeque_b->head)
		return (1);
	return (0);
}
