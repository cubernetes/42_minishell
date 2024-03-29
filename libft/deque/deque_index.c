/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_index.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:38:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/27 17:59:00 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* cmp must return 0 if comparison fails */
int	deque_index(t_deque *deque, void *cmp_ptr,
		t_bool (cmp)(void *ptr1, void *ptr2))
{
	t_deque_node	*head;
	int				idx;

	head = deque->head;
	if (!head)
		return (-1);
	if (cmp(head->as_ptr, cmp_ptr))
		return (0);
	idx = 1;
	while (head->next != deque->head)
	{
		head = head->next;
		if (cmp(head->as_ptr, cmp_ptr))
			return (idx);
		++idx;
	}
	return (-1);
}
