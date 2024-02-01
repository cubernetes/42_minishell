/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:32 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 03:41:19 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_sort(t_deque *deque, t_bool (cmp)(t_deque_type, t_deque_type))
{
	size_t	n;
	size_t	orig_n;
	size_t	i;

	orig_n = deque_size(deque);
	n = orig_n;
	if (!n)
		return ;
	while (--n)
	{
		i = orig_n;
		while (--i)
		{
			if (cmp(deque->head->data, deque->head->next->data))
				deque_swap(deque);
			deque->head = deque->head->next;
		}
		deque_rotate(deque, 1);
	}
}
