/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_deque_push_ptr_right.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:42:21 by tischmid          #+#    #+#             */
/*   Updated: 2024/04/01 01:05:16 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

/* not using perror and exit for compliance */
/* this function must not use ft_malloc (or functions that call it, like */
/* ft_memdup) */
/* it shall only be used by ft_malloc from memory management */
void	ft_malloc_deque_push_ptr_right(t_deque *deque, void *ptr)
{
	t_deque_node	*new;

	if (deque->head)
	{
		/* ft_printf("\033[31mMALLOC\033[m\n"); */
		/* print_callstack(); */
		new = malloc(sizeof(*new));
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
		/* ft_printf("\033[31mMALLOC\033[m\n"); */
		/* print_callstack(); */
		deque->head = malloc(sizeof(*deque->head));
		if (!deque->head)
			return ;
		deque->head->as_ptr = ptr;
		deque->head->next = deque->head;
		deque->head->prev = deque->head;
	}
	deque->size += 1;
}
