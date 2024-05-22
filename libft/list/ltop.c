#include "libft.h"

/** Return the last element of a list without deleting.
 *
 *  @param list: the list to get the last element of
 *  @returns: the last element
 */
t_list_node	*ltop(t_list *list)
{
	return (list->last);
}
