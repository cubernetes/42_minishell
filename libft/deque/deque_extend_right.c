/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_extend_right.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:40:27 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 05:09:37 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_extend_right(t_deque *deque_a, t_deque *deque_b)
{
	while (deque_b->head)
		deque_push_node_right(deque_a, deque_pop_left(deque_b));
}
