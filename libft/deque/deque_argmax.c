/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_argmax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:06:31 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/16 13:06:52 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <limits.h>

int	deque_argmax(t_deque *deque, int *max_idx)
{
	t_deque_node	*orig_head;
	t_deque_node	*head;
	t_deque_type	max_value;
	int				idx;

	orig_head = deque->head;
	head = orig_head;
	if (!head)
		return (*max_idx = -1, INT_MIN);
	idx = 0;
	if (max_idx)
		*max_idx = idx;
	max_value = head->data;
	head = head->next;
	while (++idx && head != orig_head)
	{
		if (head->data > max_value)
		{
			max_value = head->data;
			if (max_idx)
				*max_idx = idx;
		}
		head = head->next;
	}
	return (max_value);
}
