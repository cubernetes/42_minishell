/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 21:34:56 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

size_t	deque_size(t_deque *deque)
{
	t_di	*di;
	size_t	size;

	size = 0;
	di = di_begin(deque);
	while (di_next(di))
		++size;
	return (size);
}
