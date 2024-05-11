#include "libft.h"

/** End the iterator prematurely.
 *
 *  @param list: the list to end the iteration of
 *  @returns: `list'
 */
t_list *last(t_list list[static 1])
{
	list->current = list->current_stack->last->as_ptr;
	list->current_idx = list->current_idx_stack->last->as_size_t;
	(void)pop(list->current_stack);
	(void)pop(list->current_idx_stack);
	return (list);
}
