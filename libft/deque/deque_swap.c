/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_swap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:40:38 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 14:40:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_swap(t_deque *deque)
{
	t_deque_node	*orig_prev;
	t_deque_node	*orig_head;
	t_deque_node	*orig_next;
	t_deque_node	*orig_next_next;

	if (!deque->head)
		return ;
	if (deque->head->next->next == deque->head)
	{
		deque_rotate(deque, 1);
		return ;
	}
	orig_prev = deque->head->prev;
	orig_head = orig_prev->next;
	orig_next = orig_head->next;
	orig_next_next = orig_next->next;
	deque->head = orig_next;
	orig_prev->next = orig_next;
	orig_head->next = orig_next_next;
	orig_head->prev = orig_next;
	orig_next->next = orig_head;
	orig_next->prev = orig_prev;
	orig_next_next->prev = orig_head;
}
