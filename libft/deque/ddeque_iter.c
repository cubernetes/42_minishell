/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_iter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/07 06:28:41 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_iter(t_ddeque *ddeque, void (*f)(void *data))
{
	t_ddeque_node	*head;
	t_ddeque_node	*orig_head;

	head = ddeque->head;
	orig_head = head;
	if (!head)
		return ;
	f(head->data);
	while (head->next != orig_head)
	{
		head = head->next;
		f(head->data);
	}
}
