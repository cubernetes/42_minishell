/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_swap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:40:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:21:44 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_swap(t_ddeque *ddeque)
{
	t_ddeque_node	*orig_prev;
	t_ddeque_node	*orig_head;
	t_ddeque_node	*orig_next;
	t_ddeque_node	*orig_next_next;

	if (!ddeque->head)
		return ;
	if (ddeque->head->next->next == ddeque->head)
	{
		ddeque_rotate(ddeque, 1);
		return ;
	}
	orig_prev = ddeque->head->prev;
	orig_head = orig_prev->next;
	orig_next = orig_head->next;
	orig_next_next = orig_next->next;
	ddeque->head = orig_next;
	orig_prev->next = orig_next;
	orig_head->next = orig_next_next;
	orig_head->prev = orig_next;
	orig_next->next = orig_head;
	orig_next->prev = orig_prev;
	orig_next_next->prev = orig_head;
}
