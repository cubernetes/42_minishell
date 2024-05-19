#include "libft.h"

/** Return a data union with the same data as the data union from a list_node
 *
 *  @param list_node: the list_node* containing the data union to be extracted
 *  @returns: the data union extract from `list_node'
 */
t_data	as_data(t_list_node *list_node)
{
	return (*(t_data *)list_node);
}
