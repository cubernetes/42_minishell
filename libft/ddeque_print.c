/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:41:02 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ddeque_print(t_ddeque *ddeque, void (print)(void *data, int first))
{
	t_ddeque_node	*head;
	t_ddeque_node	*orig_head;

	head = ddeque->head;
	orig_head = head;
	if (!head && ft_printf("Empty ddeque.\n"))
		return ;
	print(head->data, 1);
	while (head->next != orig_head)
	{
		print(head->next->data, 0);
		head = head->next;
	}
	ft_printf("\n");
}
