#include "libft.h"

/** Remove the first element of a list.
 *
 *  @param list: the list to remove the first element of
 *  @returns: `list'
 */
t_list	*lpop_left(t_list *list)
{
	if (list->len == 1)
	{
		list->first = NULL;
		list->last = NULL;
		list->len--;
	}
	else if (list->len > 1)
	{
		list->first->next->prev = list->last;
		list->last->next = list->first->next;
		list->first = list->first->next;
		list->len--;
	}
	return (list);
}
/* TODO: Test rigorously */
