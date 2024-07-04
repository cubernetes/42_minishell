/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:40 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:04:40 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ht_destroy(t_ht *ht[MAX_HT_SIZE], bool (*free_data)(t_data data))
{
	int		i;
	t_ht	*prev;
	t_ht	*kv;

	i = -1;
	while (++i < MAX_HT_SIZE)
	{
		kv = ht[i];
		while (kv)
		{
			ft_free(kv->k);
			kv->k = NULL;
			free_data(kv->v);
			prev = kv;
			kv = kv->n;
			ft_free(prev);
		}
	}
}
