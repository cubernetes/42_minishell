/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:36:56 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 13:22:53 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_ddeque	*ddeque_init(void)
{
	t_ddeque	*deque;

	deque = malloc(sizeof(*deque));
	if (!deque)
		return (NULL);
	deque->head = NULL;
	deque->size = 0;
	return (deque);
}
