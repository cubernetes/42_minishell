/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_copy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:52 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/07 06:33:09 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_deque	*deque_copy(t_deque *deque)
{
	t_deque			*new_deque;
	t_deque_node	*orig_head;
	t_deque_node	*head;

	new_deque = deque_init();
	if (!deque->head)
		return (new_deque);
	orig_head = deque->head;
	head = orig_head;
	deque_push_value_bottom(new_deque, (t_deque_type)head->data);
	while (head->next != orig_head)
	{
		head = head->next;
		deque_push_value_bottom(new_deque, (t_deque_type)head->data);
	}
	return (new_deque);
}
