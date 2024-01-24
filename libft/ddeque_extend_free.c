/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_extend_free.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:40:27 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:49:20 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ddeque_extend_free(t_ddeque *ddeque_a, t_ddeque *ddeque_b,
		void (free_data)(void *))
{
	while (ddeque_b->head)
	{
		ddeque_push_node_bottom(ddeque_a, ddeque_pop_top(ddeque_b));
		ddeque_a->size += 1;
	}
	ddeque_free(ddeque_b, free_data);
}
