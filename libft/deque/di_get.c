/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   di_get.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 20:23:11 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/30 23:12:33 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* must not be called before the first di_next() */
t_deque_node	*di_get(t_di *di)
{
	if (di->head == NULL)
		return (NULL);
	else if (di->_first_iter_di_get)
		return (di->deque->head);
	else if (di->head == di->deque->head)
		return (NULL);
	else
		return (di->head);
}
