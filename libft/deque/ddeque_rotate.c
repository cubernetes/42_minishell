/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ddeque_rotate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 01:46:39 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/15 15:21:31 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ddeque_rotate(t_ddeque *ddeque, int n)
{
	if (!ddeque->head)
		return ;
	if (n > 0)
	{
		while (n--)
		{
			ddeque->head = ddeque->head->next;
		}
	}
	else if (n < 0)
		while (n++)
			ddeque->head = ddeque->head->prev;
}
