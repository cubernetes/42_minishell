#include "list.h"

/** Backwards-advance the iterator of a list.
 *
 *  @param list: the list to backwards-advance the iterator of
 *  @returns: the list node that the list's iterator points to after backwards-
 *            advancing;
 *            NULL after the last element and pops from the iterator stack
 */
t_list_node	*lprev(t_list list[static 1])
{
	if (list->current_idx == list->len - 1)
	{
		list->current = list->current_stack->last->as_ptr;
		list->current_idx = list->current_idx_stack->last->as_size_t;
		(void)lpop(list->current_stack);
		(void)lpop(list->current_idx_stack);
		return (NULL);
	}
	list->current = list->current->prev;
	list->current_idx++;
	return (list->current);
}


