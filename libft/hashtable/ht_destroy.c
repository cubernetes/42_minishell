#include "libft.h"

#include <stdlib.h>

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
			free(kv->k);
			free_data(kv->v);
			prev = kv;
			kv = kv->n;
			free(prev);
		}
	}
}
