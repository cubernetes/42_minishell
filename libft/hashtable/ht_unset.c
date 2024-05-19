#include "libft.h"

void	ht_unset(t_ht *ht[MAX_HT_SIZE], char key[static 1])
{
	int		idx;
	t_ht	*kv;
	t_ht	*prev;

	prev = NULL;
	idx = (int)(ht_hash(key) % MAX_HT_SIZE);
	kv = ht[idx];
	while (kv != NULL && kv->n != NULL && kv->k != NULL
		&& ft_strcmp(kv->k, key))
	{
		prev = kv;
		kv = kv->n;
	}
	if (kv == NULL)
		return ;
	else if (kv->k == NULL)
		return ;
	else if (ft_strcmp(kv->k, key))
		return ;
	if (kv == ht[idx])
		ht[idx] = kv->n;
	else
		prev->n = kv->n;
	(ft_free(kv->k), kv->k = NULL, ft_free(kv), kv = NULL);
}
