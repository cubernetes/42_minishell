/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_index.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:38:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:46:18 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	deque_index(t_deque *deque, t_deque_type data)
{
	t_deque_node	*head;
	int				idx;

	head = deque->head;
	if (!head)
		return (-1);
	if (head->data == data)
		return (0);
	head = head->next;
	idx = 1;
	while (head != deque->head)
	{
		if (head->data == data)
			return (idx);
		++idx;
		head = head->next;
	}
	return (-1);
}
