/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_index.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:38:41 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 14:38:49 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	deque_index(t_deque *deque, t_deque_type data)
{
	t_deque_node	*orig_head;
	t_deque_node	*head;
	int				idx;

	orig_head = deque->head;
	head = orig_head;
	if (!head)
		return (-1);
	if (head->data == data)
		return (0);
	head = head->next;
	idx = 1;
	while (head != orig_head)
	{
		if (head->data == data)
			return (idx);
		++idx;
		head = head->next;
	}
	return (-1);
}
