#include "libft.h"

/** Advance the iterator of a list.
 *
 *  @param list: the list to advance the iterator of
 *  @returns: the list node that the list's iterator points to after advancing;
 *            NULL after the last element and pops from the iterator stack
 */
t_list_node	*next(t_list list[static 1])
{
	if (list->current_idx == list->len - 1)
	{
		list->current = list->current_stack->last->as_ptr;
		list->current_idx = list->current_idx_stack->last->as_size_t;
		(void)pop(list->current_stack);
		(void)pop(list->current_idx_stack);
		return (NULL);
	}
	list->current = list->current->next;
	list->current_idx++;
	return (list->current);
}


