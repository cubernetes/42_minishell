/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_equal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:24:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 23:11:46 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* cmp must return 0 if comparison fails */
/* TODO: Not required: Implemented variadic di_next() function */
t_bool	deque_equal(t_deque *deque_a, t_deque *deque_b,
		t_bool (cmp)(void *ptr1, void *ptr2))
{
	t_di	*di_a;
	t_di	*di_b;

	di_a = di_begin(deque_a);
	di_b = di_begin(deque_b);
	(di_next(di_a), di_next(di_b));
	while (di_get(di_a) && di_get(di_b))
	{
		if (!cmp(di_get(di_a)->as_ptr, di_get(di_b)->as_ptr))
			return (FALSE);
		(di_next(di_a), di_next(di_b));
	}
	return (di_get(di_a) == NULL && di_get(di_b) == NULL);
}
