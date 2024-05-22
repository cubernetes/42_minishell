#include "libft.h"

/** Remove the last element of a list.
 *
 *  @param list: the list to remove the last element of
 *  @returns: the last element
 */
t_list_node	*lpop(t_list *list)
{
	t_list_node	*last;

	last = list->last;
	if (list->len == 1)
	{
		list->first = NULL;
		list->last = NULL;
		list->len--;
	}
	else if (list->len > 1)
	{
		list->last->prev->next = list->first;
		list->first->prev = list->last->prev;
		list->last = list->last->prev;
		list->len--;
	}
	return (last);
}
