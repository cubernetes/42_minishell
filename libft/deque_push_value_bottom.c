/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_push_value_bottom.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:42:21 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/16 16:49:43 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/* not using perror and exit for compliance */
void	deque_push_value_bottom(t_deque *deque, t_deque_type data)
{
	t_deque_node	*new;

	if (deque->head)
	{
		new = malloc(sizeof(*new));
		if (!new)
			return ;
		new->data = data;
		new->next = deque->head;
		new->prev = deque->head->prev;
		deque->head->prev->next = new;
		deque->head->prev = new;
	}
	else
	{
		deque->head = malloc(sizeof(*deque->head));
		if (!deque->head)
			return ;
		deque->head->data = data;
		deque->head->next = deque->head;
		deque->head->prev = deque->head;
	}
	deque->size += 1;
}
