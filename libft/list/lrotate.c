/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lrotate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:00:16 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:00:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* TODO: Document */
void	lrotate(t_list *list, int n)
{
	if (n > 0)
	{
		while (n--)
		{
			list->first = list->first->next;
			list->last = list->last->next;
		}
	}
	else if (n < 0)
	{
		while (n++)
		{
			list->first = list->first->prev;
			list->last = list->last->prev;
		}
	}
}
