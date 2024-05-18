#include "list.h"

/** Remove the first element of a list.
 *
 *  @param list: the list to remove the first element of
 *  @returns: `list'
 */
t_list	*lpop_left(t_list *list)
{
	list->first->next->prev = list->last;
	list->last->next = list->first->next;
	list->first = list->first->next;
	list->len--;
	return (list);
}
/* TODO: Test rigorously */
