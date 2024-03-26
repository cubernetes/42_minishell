/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:49:02 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_print(t_ddeque *ddeque, void (print)(void *data, t_bool first))
{
	t_ddeque_node	*head;

	head = ddeque->head;
	if (!head && ft_printf("Empty ddeque.\n"))
		return ;
	print(head->data, TRUE);
	while (head->next != ddeque->head)
	{
		head = head->next;
		print(head->data, FALSE);
	}
	ft_printf("\n");
}
