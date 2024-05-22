#include "libft.h"

/** Advance the iterator of a list using the defined method.
 *
 *  @param list: the list to advance the iterator of
 *  @returns: the list node that the list's iterator points to after advancing;
 *            NULL after the last element and pops from the iterator stack
 */
t_list_node	*lnext(t_list list[static 1])
{
	if (list->_iterator_stack->len == 0)
		return (NULL);
	return (list->_method(list));
}
/* TODO: Edge case? calling lnext when there was never any iterator */
