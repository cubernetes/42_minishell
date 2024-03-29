/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_sum.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/27 17:52:56 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	deque_sum(t_deque *deque, int (*f)(void *ptr))
{
	t_deque_node	*head;
	int				sum;

	sum = 0;
	head = deque->head;
	if (!head)
		return (0);
	sum += f(head->as_ptr);
	while (head->next != deque->head)
	{
		head = head->next;
		sum += f(head->as_ptr);
	}
	return (sum);
}
