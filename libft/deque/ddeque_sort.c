/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:32 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:42:09 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_sort(t_ddeque *ddeque, int (cmp)(void *, void *))
{
	size_t	n;
	size_t	orig_n;
	size_t	i;

	orig_n = ddeque_size(ddeque);
	n = orig_n;
	if (!n)
		return ;
	while (--n)
	{
		i = orig_n;
		while (--i)
		{
			if (cmp(ddeque->head->data, ddeque->head->next->data))
				ddeque_swap(ddeque);
			ddeque->head = ddeque->head->next;
		}
		ddeque_rotate(ddeque, 1);
	}
}
