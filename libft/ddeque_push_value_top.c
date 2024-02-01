/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_push_value_top.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:34:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/01 07:24:34 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/* not using perror and exit for compliance */
void	ddeque_push_value_top(t_ddeque *ddeque, void *data)
{
	t_ddeque_node	*new;

	if (ddeque->head)
	{
		new = malloc(sizeof(*new));
		if (!new)
			return ;
		DEBUG(3, "CREATED NEW VALUE %p", new);
		DEBUG(3, "4. changing %p to %p", ddeque->head->prev->next, new);
		ddeque->head->prev->next = new;
		DEBUG(3, "5. changing %p to %p", ddeque->head->prev, new);
		ddeque->head->prev = new;
		DEBUG(3, "6. changing %p to %p", ddeque->head, new);
		ddeque->head = new;
	}
	else
	{
		DEBUG(3, "CREATING NEW HEAD", "");
		ddeque->head = malloc(sizeof(*ddeque->head));
		if (!ddeque->head)
			return ;
		ddeque->head->data = data;
		ddeque->head->next = ddeque->head;
		ddeque->head->prev = ddeque->head;
	}
	ddeque->size += 1;
}
