/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_push_value_top.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:34:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/01 07:34:56 by tosuman          ###   ########.fr       */
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
		ddeque->head->prev->next = new;
		ddeque->head->prev = new;
		ddeque->head = new;
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
