/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_index.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:38:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 03:42:19 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* cmp must return 0 if values are not equal */
int	ddeque_index(t_ddeque *ddeque, void *data, t_bool (cmp)(void *, void *))
{
	t_ddeque_node	*orig_head;
	t_ddeque_node	*head;
	int				idx;

	orig_head = ddeque->head;
	head = orig_head;
	if (!head)
		return (-1);
	if (cmp(head->data, data))
		return (0);
	head = head->next;
	idx = 1;
	while (head != orig_head)
	{
		if (cmp(head->data, data))
			return (idx);
		++idx;
		head = head->next;
	}
	return (-1);
}
