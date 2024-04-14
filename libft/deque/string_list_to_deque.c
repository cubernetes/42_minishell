/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_list_to_deque.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:25:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/04/13 22:34:06 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_deque	*string_list_to_deque(char **array_list, void *(new_node_ptr)(char *str))
{
	t_deque	*deque;

	deque = deque_init();
	while (*array_list)
	{
		deque_push_ptr_right(deque, new_node_ptr(*array_list));
		++array_list;
	}
	return (deque);
}
