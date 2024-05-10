/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   di_begin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 13:36:45 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/30 22:46:36 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_di	*di_begin(t_deque *deque)
{
	return (ft_memdup(\
		&(t_di){
			.deque = deque,
			.head = deque->head,
			._first_iter = deque->head != NULL,
			._first_iter_di_get = deque->head != NULL
		},
		sizeof(t_di)
	));
}
