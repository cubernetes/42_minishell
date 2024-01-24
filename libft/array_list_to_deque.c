/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list_to_deque.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 23:24:21 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_deque	*array_list_to_deque(char **array_list, int *status)
{
	t_deque	*deque;

	deque = deque_init();
	while (*array_list)
	{
		deque_push_value_bottom(deque,
			(t_deque_type)ft_atoi_status(*array_list, status));
		++array_list;
	}
	return (deque);
}
