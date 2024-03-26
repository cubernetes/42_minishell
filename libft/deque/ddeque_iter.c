/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_iter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:45:25 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_iter(t_ddeque *ddeque, void (*f)(void *data))
{
	t_ddeque_node	*head;

	head = ddeque->head;
	if (!head)
		return ;
	f(head->data);
	while (head->next != ddeque->head)
	{
		head = head->next;
		f(head->data);
	}
}
