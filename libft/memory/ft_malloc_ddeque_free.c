/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_ddeque_free.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:19 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/02 04:59:47 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

/* only function allowd to use free */
/* must only be used by ft_malloc from memory management */
t_bool	ft_malloc_ddeque_free(t_ddeque *ddeque, t_bool (free_data)(void *))
{
	t_ddeque_node	*head;
	t_ddeque_node	*tail;

	if (!ddeque)
		return (TRUE);
	head = ddeque->head;
	if (!head)
	{
		(free(ddeque), ddeque = NULL);
		return (TRUE);
	}
	tail = head->prev;
	while (head != tail)
	{
		head = head->next;
		(void)free_data(head->prev->data);
		(free(head->prev), head->prev = NULL);
	}
	free_data(head->data);
	(free(head), head = NULL);
	(free(ddeque), ddeque = NULL);
	return (TRUE);
}
