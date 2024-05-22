#include "libft.h"

/** Returns the first element of a list.
 *
 *  @param list: the list to get the first element of
 *  @returns: the first element
 */
t_list_node	*ltop_left(t_list *list)
{
	return (list->first);
}
