/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 17:14:50 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stddef.h>

size_t	deque_size(t_deque *deque)
{
	t_deque_node	*head;
	size_t			size;

	size = 0;
	head = deque->head;
	while (head && (head->next != deque->head || !first++))
	{
		++size;
		head = head->next;
	}
	return (size);
}
