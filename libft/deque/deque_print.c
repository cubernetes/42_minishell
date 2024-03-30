/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 23:09:25 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	deque_print(t_deque *deque, void (print)(void *ptr, t_bool first))
{
	t_di	*di;

	di = di_begin(deque);
	(void)di_next(di);
	if (di_get(di) == NULL && ft_printf(EMPTY_DEQUE))
		return ;
	print(di_get(di)->as_ptr, TRUE);
	while (di_next(di))
		print(di_get(di)->as_ptr, FALSE);
	ft_printf("\n");
}
