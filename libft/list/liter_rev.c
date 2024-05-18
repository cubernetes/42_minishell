#include "list.h"

/** Start a new backwards-iterator on a list.
 *
 *  @param list: the list to start the backwards-iterator on
 *  @returns: `list'
 */
t_list	*liter_rev(t_list list[static 1])
{
	(void)lpush(list->current_stack, as_ptr(list->current));
	(void)lpush(list->current_idx_stack, as_size_t(list->current_idx));
	if (list->last)
		list->current = list->last->next;
	list->current_idx = (size_t)-1;
	return (list);
}
