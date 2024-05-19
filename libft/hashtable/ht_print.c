#include "libft.h"

/* print() must not print any newlines */
void	ht_print(t_ht ht[MAX_HT_SIZE], void (print)(char *k, t_data v))
{
	int		i;
	t_ht	*kv;
	int		j;

	i = -1;
	while (++i < MAX_HT_SIZE)
	{
		ft_printf("%d: ", i);
		kv = &ht[i];
		j = 0;
		while (1)
		{
			if (!kv || !kv->k)
				break ;
			if (j++ > 0)
				ft_printf(" -> ");
			print(kv->k, kv->v);
			kv = kv->n;
		}
		ft_printf("\n");
	}
}
