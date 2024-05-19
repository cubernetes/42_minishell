#include "libft.h"

void	ht_set(t_ht *ht[MAX_HT_SIZE], char key[static 1], t_data value)
{
	int		idx;
	t_ht	*new_kv;
	t_ht	*kv;

	idx = (int)(ht_hash(key) % MAX_HT_SIZE);
	kv = ht[idx];
	while (kv != NULL && kv->n != NULL && kv->k != NULL
		&& ft_strcmp(kv->k, key))
		kv = kv->n;
	if (kv != NULL)
	{
		if (kv->k != NULL && ft_strcmp(kv->k, key))
		{
			kv->v = value;
			return ;
		}
	}
	new_kv = ft_malloc(sizeof(*new_kv));
	new_kv->k = ft_strdup(key);
	new_kv->v = value;
	new_kv->n = NULL;
	if (kv == NULL)
		ht[idx] = new_kv;
	else
		kv->n = new_kv;
}
