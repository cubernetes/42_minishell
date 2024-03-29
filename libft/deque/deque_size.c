/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:47:03 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stddef.h>

size_t	deque_size(t_deque *deque)
{
	t_deque_node	*head;
	size_t			size;

	head = deque->head;
	if (!head)
		return (0);
	size = 1;
	while (head->next != deque->head)
	{
		head = head->next;
		++size;
	}
	return (size);
}
