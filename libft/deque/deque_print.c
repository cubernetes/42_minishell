/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:37:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/05/10 04:12:59 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

void	deque_print(t_deque *deque, void (print)(void *ptr, bool first))
{
	t_di	*di;

	di = di_begin(deque);
	(void)di_next(di);
	if (di_get(di) == NULL && ft_printf(EMPTY_DEQUE))
		return ;
	print(di_get(di)->as_ptr, true);
	while (di_next(di))
		print(di_get(di)->as_ptr, false);
	ft_printf("\n");
}
