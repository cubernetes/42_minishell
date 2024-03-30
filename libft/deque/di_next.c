/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   di_next.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:39:54 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/30 23:36:39 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_deque_node	*di_next(t_di *di)
{
	if (di->head == NULL)
		return (NULL);
	else if (di->_first_iter)
	{
		di->_first_iter = FALSE;
		return (di->head);
	}
	else
	{
		di->_first_iter_di_get = FALSE;
		di->head = di->head->next;
		if (di->head == di->deque->head)
			di->head = NULL;
		return (di->head);
	}
}
