/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_begin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:36:45 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/28 13:38:04 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

t_deque_iter	*deque_begin(t_deque *deque)
{
	t_deque_iter	*deque_iter;

	deque_iter = malloc(sizeof(*deque_iter));
	deque_iter->deque = deque;
	deque_iter->head = deque->head;
	deque_iter->first_iter = TRUE;
	return (deque_iter);
}
