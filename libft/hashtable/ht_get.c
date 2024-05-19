#include "libft.h"

t_data	ht_get(t_ht *ht[MAX_HT_SIZE], char key[static 1])
{
	t_ht	*kv;

	kv = ht[ht_hash(key) % MAX_HT_SIZE];
	while (kv && kv->k && ft_strcmp(kv->k, key))
		kv = kv->n;
	if (kv && kv->k)
		return (kv->v);
	return ((t_data){0});
}
