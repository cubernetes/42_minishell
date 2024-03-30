/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_deque_init.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:36:56 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 22:10:05 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

/* this function must not use ft_malloc (or functions that call it) */
/* it shall only be used by ft_malloc from memory management */
t_deque	*ft_malloc_deque_init(void)
{
	t_deque	*deque;

	deque = malloc(sizeof(*deque));
	if (!deque)
		return (NULL);
	deque->head = NULL;
	deque->size = 0;
	return (deque);
}
