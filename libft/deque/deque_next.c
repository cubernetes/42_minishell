/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_next.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:39:54 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/28 13:42:43 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_deque_node	*deque_next(t_deque_iter	*deque_iter)
{
	if (deque_iter->head == NULL)
		return (NULL);
	else if (deque_iter->first_iter)
	{
		deque_iter->first_iter = FALSE;
		return (deque_iter->deque->head);
	}
	else
	{
		deque_iter->head = deque_iter->head->next;
		if (deque_iter->head == deque_iter->deque->head)
			return (NULL);
		else
			return (deque_iter->head);
	}
}
