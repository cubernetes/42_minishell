/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lextend_left.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:00:22 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:00:23 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	lextend_left(t_list *list_a, t_list *list_b)
{
	while (list_b->first)
		(lpush_left(list_a, as_data(list_b->last)), lpop(list_b));
}
