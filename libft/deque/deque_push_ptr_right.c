/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_push_ptr_right.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:42:21 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/27 17:57:59 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

/* not using perror and exit for compliance */
void	deque_push_ptr_right(t_deque *deque, void *ptr)
{
	t_deque_node	*new;

	if (deque->head)
	{
		new = ft_malloc(sizeof(*new));
		if (!new)
			return ;
		new->as_ptr = ptr;
		new->next = deque->head;
		new->prev = deque->head->prev;
		deque->head->prev->next = new;
		deque->head->prev = new;
	}
	else
	{
		deque->head = ft_malloc(sizeof(*deque->head));
		if (!deque->head)
			return ;
		deque->head->as_ptr = ptr;
		deque->head->next = deque->head;
		deque->head->prev = deque->head;
	}
	deque->size += 1;
}
