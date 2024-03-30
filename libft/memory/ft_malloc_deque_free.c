/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_deque_free.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:19 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 21:56:05 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

/* only function allowd to use free */
/* must only be used by ft_malloc from memory management */
/* cannot use iterators (t_di) here */
t_bool	ft_malloc_deque_free(t_deque *deque, t_bool (free_data)(void *ptr))
{
	t_deque_node	*head;
	t_deque_node	*tail;

	if (!deque)
		return (TRUE);
	head = deque->head;
	if (!head)
	{
		(free(deque), deque = NULL);
		return (TRUE);
	}
	tail = head->prev;
	while (head != tail)
	{
		head = head->next;
		(void)free_data(head->prev->as_ptr);
		(free(head->prev), head->prev = NULL);
	}
	free_data(head->as_ptr);
	(free(head), head = NULL);
	(free(deque), deque = NULL);
	return (TRUE);
}
