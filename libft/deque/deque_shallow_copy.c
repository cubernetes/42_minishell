/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_shallow_copy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:52 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 21:33:47 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_deque	*deque_shallow_copy(t_deque *deque)
{
	t_deque	*new_deque;
	t_di	*di;

	new_deque = deque_init();
	di = di_begin(deque);
	while (di_next(di))
		deque_push_ptr_right(new_deque, di_get(di)->as_ptr);
	return (new_deque);
}
