/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_size.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:47:03 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stddef.h>

size_t	ddeque_size(t_ddeque *ddeque)
{
	t_ddeque_node	*head;
	size_t			size;

	head = ddeque->head;
	if (!head)
		return (0);
	size = 1;
	while (head->next != ddeque->head)
	{
		head = head->next;
		++size;
	}
	return (size);
}
