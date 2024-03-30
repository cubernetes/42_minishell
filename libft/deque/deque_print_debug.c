/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print_debug.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 17:50:23 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/30 23:09:08 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_print_debug(t_deque *deque)
{
	t_di	*di;

	di = di_begin(deque);
	(void)di_next(di);
	if (di_get(di) == NULL && ft_printf(EMPTY_DEQUE))
		return ;
	ft_printf("<\033[31m%p\033[m:\033[32m%p\033[m:\033[33m%p\033[m>",
		di_get(di)->prev, di_get(di), di_get(di)->next);
	while (di_next(di))
		ft_printf("\n<\033[31m%p\033[m:\033[32m%p\033[m:\033[33m%p\033[m>",
			di_get(di)->prev, di_get(di), di_get(di)->next);
	ft_printf("\n");
}
