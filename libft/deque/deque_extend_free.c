/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_extend_free.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:40:27 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 01:31:23 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_extend_free(t_deque *deque_a, t_deque *deque_b)
{
	while (deque_b->head)
	{
		deque_push_node_bottom(deque_a, deque_pop_top(deque_b));
		deque_a->size += 1;
	}
	(void)deque_free(deque_b);
}
