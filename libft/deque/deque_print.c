/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/07 06:28:04 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_print(t_deque *deque)
{
	t_deque_node	*head;
	t_deque_node	*orig_head;

	head = deque->head;
	orig_head = head;
	if (!head && ft_printf("Empty deque.\n"))
		return ;
	ft_printf("%d", head->data);
	while (head->next != orig_head)
	{
		head = head->next;
		ft_printf(" -> %d", head->data);
	}
	ft_printf("\n");
}
