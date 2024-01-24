/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_push_node_top.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:42:36 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 21:42:00 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ddeque_push_node_top(t_ddeque *ddeque, t_ddeque_node *node)
{
	if (!node)
		return ;
	if (ddeque->head)
	{
		node->next = ddeque->head;
		node->prev = ddeque->head->prev;
		ddeque->head->prev->next = node;
		ddeque->head->prev = node;
		ddeque->head = node;
	}
	else
	{
		ddeque->head = node;
		ddeque->head->next = ddeque->head;
		ddeque->head->prev = ddeque->head;
	}
	ddeque->size += 1;
}
