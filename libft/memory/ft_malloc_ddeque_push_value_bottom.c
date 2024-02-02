/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_ddeque_push_value_bottom.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:42:21 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/02 04:59:31 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

void	print_ptr(void *data, t_bool first)
{
	if (first)
		ft_printf("%p", data);
	else
		ft_printf(" -> %p", data);
}

/* not using perror and exit for compliance */
/* this function must not use ft_malloc */
/* it shall only be used by ft_malloc from memory management */
void	ft_malloc_ddeque_push_value_bottom(t_ddeque *ddeque, void *data)
{
	t_ddeque_node	*new;

	if (ddeque->head)
	{
		new = malloc(sizeof(*new));
		if (!new)
			return ;
		new->data = data;
		new->next = ddeque->head;
		new->prev = ddeque->head->prev;
		ddeque->head->prev->next = new;
		ddeque->head->prev = new;
	}
	else
	{
		ddeque->head = malloc(sizeof(*ddeque->head));
		if (!ddeque->head)
			return ;
		ddeque->head->data = data;
		ddeque->head->next = ddeque->head;
		ddeque->head->prev = ddeque->head;
	}
	ddeque->size += 1;
}
