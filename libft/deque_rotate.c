/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_rotate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 01:46:39 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 14:40:35 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	deque_rotate(t_deque *deque, int n)
{
	if (!deque->head)
		return ;
	if (n > 0)
	{
		while (n--)
		{
			deque->head = deque->head->next;
		}
	}
	else if (n < 0)
		while (n++)
			deque->head = deque->head->prev;
}
