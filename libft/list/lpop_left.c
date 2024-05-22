#include "libft.h"

/** Remove the first element of a list.
 *
 *  @param list: the list to remove the first element of
 *  @returns: the first element
 */
t_list_node	*lpop_left(t_list *list)
{
	t_list_node	*first;

	first = list->first;
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
	return (first);
}
/* TODO: Test rigorously */
