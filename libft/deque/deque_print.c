/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/26 08:44:13 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_print(t_deque *deque)
{
	t_deque_node	*head;

	head = deque->head;
	if (!head && ft_printf("Empty deque.\n"))
		return ;
	ft_printf("%d", head->data);
	while (head->next != deque->head)
	{
		head = head->next;
		ft_printf(" -> %d", head->data);
	}
	ft_printf("\n");
}
