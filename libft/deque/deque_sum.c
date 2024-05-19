/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_sum.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 21:37:54 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	deque_sum(t_deque *deque, int (*f)(void *ptr))
{
	t_di	*di;
	int		sum;

	sum = 0;
	di = di_begin(deque);
	while (di_next(di))
		sum += f(di_get(di)->as_ptr);
	return (sum);
}
