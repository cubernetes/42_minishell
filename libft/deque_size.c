/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 14:39:47 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

size_t	deque_size(t_deque *deque)
{
	t_deque_node	*orig_head;
	t_deque_node	*head;
	size_t			size;

	orig_head = deque->head;
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
