/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/27 17:48:49 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_print(t_deque *deque, void (print)(void *ptr, t_bool first))
{
	t_deque_node	*head;

	head = deque->head;
	if (!head && ft_printf("Empty deque.\n"))
		return ;
	print(head->as_ptr, TRUE);
	while (head->next != deque->head)
	{
		head = head->next;
		print(head->as_ptr, FALSE);
	}
	ft_printf("\n");
}
