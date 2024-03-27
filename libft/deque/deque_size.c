/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/27 14:59:44 by pgrussin         ###   ########.fr       */
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
	/*changed from !first++ to !size*/
	while (head && (head->next != deque->head || !size++))
	{
		head = head->next;
	}
	return (size);
}
