/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_slice.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:06 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/25 22:14:38 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

/* also determines whether to return early */
static int	calculate_bounds(int *start, int *end, int *step, int *size)
{
	int	orig_start;

	orig_start = *start;
	if (*start < 0)
		*start = ft_max(*start + *size, 0);
	*start = ft_min(*start, *size - 1);
	if (*end < 0)
		*end = ft_max(*end + *size, 0);
	*end = ft_min(*end, *size);
	if (orig_start < 0 && *end == 0 && *step > 0)
		*end = *size;
	else if ((*end - *start) * *step <= 0)
		return (1);
	return (0);
}

t_ddeque	*ddeque_slice(t_ddeque *ddeque, int start, int end, int step)
{
	t_ddeque_node	*head;
	t_ddeque		*slice;
	int				size;

	slice = ddeque_init();
	size = (int)ddeque->size;
	if (calculate_bounds(&start, &end, &step, &size))
		return (slice);
	(free(NULL), head = ddeque->head, size = start);
	while (size--)
		head = head->next;
	while ((step > 0 && start < end) || (step < 0 && start > end))
	{
		ddeque_push_value_bottom(slice, head->data);
		size = (int)ft_abs(step);
		while (size--)
		{
			if (step > 0)
				head = head->next;
			else
				head = head->prev;
		}
		start += step;
	}
	return (slice);
}
