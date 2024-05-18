#include "libft.h"

/** Start a new iterator on a list.
 *
 *  @param list: the list to start the iterator on
 *  @returns: `list'
 */
t_list	*liter(t_list list[static 1])
{
	(void)lpush(list->current_stack, as_ptr(list->current));
	(void)lpush(list->current_idx_stack, as_size_t(list->current_idx));
	if (list->first)
		list->current = list->first->prev;
	list->current_idx = (size_t)-1;
	return (list);
}
