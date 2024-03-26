/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_sum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:48:02 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ddeque_sum(t_ddeque *ddeque, int (*f)(void *data))
{
	t_ddeque_node	*head;
	int				sum;

	sum = 0;
	head = ddeque->head;
	if (!head)
		return (0);
	sum += f(head->data);
	while (head->next != ddeque->head)
	{
		head = head->next;
		sum += f(head->data);
	}
	return (sum);
}
