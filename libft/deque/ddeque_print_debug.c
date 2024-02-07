/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_print_debug.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 17:50:23 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/03 17:50:44 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_print_debug(t_ddeque *ddeque)
{
	t_ddeque_node	*head;
	t_ddeque_node	*orig_head;

	head = ddeque->head;
	orig_head = head;
	if (!head && ft_printf("Empty ddeque.\n"))
		return ;
	ft_printf("<\033[31m%p\033[m:\033[32m%p\033[m:\033[33m%p\033[m>",
		head->prev, head, head->next);
	while (head->next != orig_head)
	{
		head = head->next;
		ft_printf("\n<\033[31m%p\033[m:\033[32m%p\033[m:\033[33m%p\033[m>",
			head->prev, head, head->next);
	}
	ft_printf("\n");
}
