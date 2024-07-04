/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_to_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:40 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:04:41 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ht_to_list(t_ht *ht[MAX_HT_SIZE])
{
	int		i;
	t_ht	*kv;
	t_list	*list;

	list = lnew();
	i = -1;
	while (++i < MAX_HT_SIZE)
	{
		kv = ht[i];
		while (kv)
		{
			(void)lpush(list, as_kv_pair(ft_memdup(&(t_kv_pair){\
					.k = kv->k, \
					.v = kv->v \
				}, \
				sizeof(t_kv_pair) \
			)));
			kv = kv->n;
		}
	}
	return (list);
}
