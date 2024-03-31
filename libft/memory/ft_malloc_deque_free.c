/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_deque_free.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:19 by tischmid          #+#    #+#             */
/*   Updated: 2024/04/01 01:04:59 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

/* only function (along ft_tree) allowd to use free */
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
		/* ft_printf("\033[32mFREE\033[m\n"); */
		/* print_callstack(); */
		(free(deque), deque = NULL);
		return (TRUE);
	}
	tail = head->prev;
	while (head != tail)
	{
		head = head->next;
		(void)free_data(head->prev->as_ptr);
		/* ft_printf("\033[32mFREE\033[m\n"); */
		/* print_callstack(); */
		(free(head->prev), head->prev = NULL);
	}
	free_data(head->as_ptr);
	/* ft_printf("\033[32mFREE\033[m\n"); */
	/* print_callstack(); */
	(free(head), head = NULL);
	/* ft_printf("\033[32mFREE\033[m\n"); */
	/* print_callstack(); */
	(free(deque), deque = NULL);
	return (TRUE);
}
