#include "list.h"
#include <stdbool.h>

/** Sort a list in-place
 *
 *  @param list: the list to sort
 *  @param cmp: a comparison function that takes two data unions and returns
 *              a bool
 */
void	lsort(t_list *list, bool (cmp)(t_data data1, t_data data2))
{
	size_t	i;
	size_t	j;

	i = list->len;
	while (i--)
	{
		j = list->len;
		while (--j)
		{
			if (cmp(as_data(list->first), as_data(list->first->next)))
				lswap(list);
			lrotate(list, 1);
		}
		lrotate(list, 1);
	}
}
