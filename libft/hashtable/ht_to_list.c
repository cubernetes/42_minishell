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
