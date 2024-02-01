/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 03:35:17 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_print(t_ddeque *ddeque, void (print)(void *data, t_bool first))
{
	t_ddeque_node	*head;
	t_ddeque_node	*orig_head;

	head = ddeque->head;
	orig_head = head;
	if (!head && ft_printf("Empty ddeque.\n"))
		return ;
	print(head->data, TRUE);
	while (head->next != orig_head)
	{
		print(head->next->data, FALSE);
		head = head->next;
	}
	ft_printf("\n");
}
