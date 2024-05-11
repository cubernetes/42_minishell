#include "libft.h"

/** Free inherent resources of a simple list (allocated pointers are not freed).
 *
 *  @param list: the simple list to destroy
 */
static void	destroy_simple_list(t_list list[static 1])
{
	struct s_list_node	*head;

	if (list->first == NULL)
	{
		free(list);
		return ;
	}
	head = list->first->next;
	while (head != list->first)
	{
		head = head->next;
		if (head->prev->type == STR)
			free(head->prev->as_str);
		free(head->prev);
	}
	if (head->type == STR)
		free(head->as_str);
	free(head);
	free(list);
}

/** Free inherent resources of a list (allocated pointers are not freed).
 *
 *  @param list: the list to destroy
 */
void	destroy_list(t_list list[static 1])
{
	destroy_simple_list(list->current_stack);
	destroy_simple_list(list->current_idx_stack);
	destroy_simple_list(list);
}
