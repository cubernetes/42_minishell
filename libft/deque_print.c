/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 14:37:53 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	deque_print(t_deque *deque)
{
	t_deque_node	*head;
	t_deque_node	*orig_head;

	head = deque->head;
	orig_head = head;
	if (head)
		ft_printf("%d", head->data);
	else
	{
		ft_printf("Empty deque.\n");
		return ;
	}
	while (head->next != orig_head)
	{
		ft_printf(" -> %d", head->next->data);
		head = head->next;
	}
	ft_printf("\n");
}
