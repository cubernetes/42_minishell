/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:36:56 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 20:36:55 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

t_deque	*deque_init(void)
{
	return (ft_memdup(\
		&(t_deque){
			.head = NULL,
			.size = 0
		},
		sizeof(t_deque)
	));
}
