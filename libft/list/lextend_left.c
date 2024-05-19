#include "libft.h"

void	lextend_left(t_list *list_a, t_list *list_b)
{
	while (list_b->first)
		(lpush_left(list_a, as_data(list_b->last)), lpop(list_b));
}
