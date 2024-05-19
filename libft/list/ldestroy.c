#include "libft.h"
#include "libft.h"

/** Free inherent resources of a simple list (allocated pointers are not freed).
 *
 *  @param list: the simple list to destroy
 */
static void	lsimple_destroy(t_list list[static 1])
{
	struct s_list_node	*head;

	if (list->first == NULL)
	{
		ft_free(list);
		return ;
	}
	head = list->first->next;
	while (head != list->first)
	{
		head = head->next;
		if (head->prev->type == TYPE_GC_PTR)
			ft_free(head->prev->as_ptr);
		ft_free(head->prev);
	}
	if (head->type == TYPE_GC_PTR)
		ft_free(head->as_ptr);
	ft_free(head);
	ft_free(list);
}

/** Free inherent resources of a list (allocated pointers are not freed).
 *
 *  @param list: the list to destroy
 */
void	ldestroy(t_list list[static 1])
{
	lsimple_destroy(list->_iterator_stack);
	lsimple_destroy(list);
}
