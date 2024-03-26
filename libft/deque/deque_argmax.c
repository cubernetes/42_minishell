/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_argmax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:06:31 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/26 08:47:42 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <limits.h>

int	deque_argmax(t_deque *deque, int *max_idx)
{
	t_deque_node	*head;
	t_deque_type	max_value;
	int				idx;

	head = deque->head;
	if (!head)
		return (*max_idx = -1, INT_MIN);
	idx = 0;
	if (max_idx)
		*max_idx = idx;
	max_value = head->data;
	while (++idx && head->next != deque->head)
	{
		head = head->next;
		if (head->data > max_value)
		{
			max_value = head->data;
			if (max_idx)
				*max_idx = idx;
		}
	}
	return (max_value);
}
