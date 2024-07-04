/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lextend.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:00:25 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:00:25 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	lextend(t_list *list_a, t_list *list_b)
{
	while (list_b->first)
		(lpush(list_a, as_data(list_b->first)), lpop_left(list_b));
}
