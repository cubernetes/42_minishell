/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print_debug.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 17:50:23 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/26 08:48:22 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_print_debug(t_deque *deque)
{
	t_deque_node	*head;

	head = deque->head;
	if (!head && ft_printf("Empty deque.\n"))
		return ;
	ft_printf("<\033[31m%p\033[m:\033[32m%p\033[m:\033[33m%p\033[m>",
		head->prev, head, head->next);
	while (head->next != deque->head)
	{
		head = head->next;
		ft_printf("\n<\033[31m%p\033[m:\033[32m%p\033[m:\033[33m%p\033[m>",
			head->prev, head, head->next);
	}
	ft_printf("\n");
}
