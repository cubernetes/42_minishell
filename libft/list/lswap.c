#include "libft.h"

/* TODO: Document */
void	lswap(t_list *list)
{
	t_list_node	*orig_prev;
	t_list_node	*orig_first;
	t_list_node	*orig_next;
	t_list_node	*orig_next_next;

	if (!list->first)
		return ;
	if (list->first->next->next == list->first)
	{
		lrotate(list, 1);
		return ;
	}
	orig_prev = list->first->prev;
	orig_first = orig_prev->next;
	orig_next = orig_first->next;
	orig_next_next = orig_next->next;
	list->first = orig_next;
	orig_prev->next = orig_next;
	orig_first->next = orig_next_next;
	orig_first->prev = orig_next;
	orig_next->next = orig_first;
	orig_next->prev = orig_prev;
	orig_next_next->prev = orig_first;

	/* TODO: Check that last and current pointers are correct */
	list->last->next = list->first;
	list->current = list->first;
}
