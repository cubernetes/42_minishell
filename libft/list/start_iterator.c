#include "libft.h"

/** Start a new iterator on a list.
 *
 *  @param list: the list to start the iterator on
 *  @returns: `list'
 */
t_list	*start_iterator(t_list list[static 1])
{
	(void)push(list->current_stack, as_ptr(list->current));
	(void)push(list->current_idx_stack, as_size_t(list->current_idx));
	if (list->first)
		list->current = list->first->prev;
	list->current_idx = (size_t)-1;
	return (list);
}
