#include "libft.h"

/** Remove the last element of a list.
 *
 *  @param list: the list to remove the last element of
 *  @returns: `list'
 */
t_list	*lpop(t_list *list)
{
	list->last->prev->next = list->first;
	list->first->prev = list->last->prev;
	list->last = list->last->prev;
	list->len--;
	return (list);
}
