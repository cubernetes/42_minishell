#include "libft.h"

/** Return a data union initialized with the char* argument.
 *
 *  @param as_str: the char* member of the union
 *  @returns: the data union containing `as_str'
 */
t_data	as_str(char *as_str)
{
	return ((t_data){.as_str = as_str, .type = STR});
}

/** Return a data union initialized with the void* argument.
 *
 *  @param as_ptr: the void* member of the union
 *  @returns: the data union containing `as_ptr'
 */
t_data	as_ptr(void *as_ptr)
{
	return ((t_data){.as_ptr = as_ptr, .type = PTR});
}

/** Return a data union initialized with the t_list_node* argument.
 *
 *  @param as_list_node: the t_list_node* member of the union
 *  @returns: the data union containing `as_list_node'
 */
t_data	as_list_node(t_list_node *as_list_node)
{
	return ((t_data){.as_list_node = as_list_node, .type = LIST_NODE});
}
