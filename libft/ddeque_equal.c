/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_equal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:24:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 03:43:07 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* cmp must return 0 if values are not equal */
t_bool	ddeque_equal(t_ddeque *ddeque_a, t_ddeque *ddeque_b,
		t_bool (cmp)(void *, void *))
{
	t_ddeque_node	*head_a;
	t_ddeque_node	*head_b;

	if (ddeque_a->size != ddeque_b->size)
		return (FALSE);
	if (ddeque_a->size == 0)
		return (TRUE);
	head_a = ddeque_a->head->next;
	head_b = ddeque_b->head->next;
	while (head_a != ddeque_a->head && head_b != ddeque_b->head)
	{
		if (!cmp(head_a->data, head_b->data))
			return (FALSE);
		head_a = head_a->next;
		head_b = head_b->next;
	}
	if (head_a == ddeque_a->head && head_b == ddeque_b->head)
		return (TRUE);
	return (FALSE);
}
