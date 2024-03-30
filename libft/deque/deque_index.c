/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_index.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:38:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 20:34:45 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* cmp must return 0 if comparison fails */
int	deque_index(t_deque *deque, void *cmp_ptr,
		t_bool (cmp)(void *ptr1, void *ptr2))
{
	t_di	*di;
	int		idx;

	idx = -1;
	di = di_begin(deque);
	while (di_next(di))
		if ((++idx, TRUE) && cmp(di_get(di)->as_ptr, cmp_ptr))
			return (idx);
	return (-1);
}
