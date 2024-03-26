/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_push_node_left.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:42:36 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 21:41:14 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_push_node_left(t_deque *deque, t_deque_node *node)
{
	if (!node)
		return ;
	if (deque->head)
	{
		node->next = deque->head;
		node->prev = deque->head->prev;
		deque->head->prev->next = node;
		deque->head->prev = node;
		deque->head = node;
	}
	else
	{
		deque->head = node;
		deque->head->next = deque->head;
		deque->head->prev = deque->head;
	}
	deque->size += 1;
}
