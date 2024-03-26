/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_extend_left.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:40:27 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 05:09:09 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_extend_left(t_ddeque *ddeque_a, t_ddeque *ddeque_b)
{
	while (ddeque_b->head)
		ddeque_push_node_left(ddeque_a, ddeque_pop_right(ddeque_b));
}
