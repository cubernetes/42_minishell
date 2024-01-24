/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_size.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:41:44 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

size_t	ddeque_size(t_ddeque *ddeque)
{
	t_ddeque_node	*orig_head;
	t_ddeque_node	*head;
	size_t			size;

	orig_head = ddeque->head;
	head = orig_head;
	if (!head)
		return (0);
	head = head->next;
	size = 1;
	while (head != orig_head)
	{
		head = head->next;
		++size;
	}
	return (size);
}
