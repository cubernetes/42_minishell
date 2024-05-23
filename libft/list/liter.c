#include "libft.h"
#include "libft.h"

/** Start a new forward-iterator on a list.
 *
 *  @param list: the list to start the forward-iterator on
 *  @returns: `list'
 */
t_list	*liter(t_list list[static 1])
{
	(void)lpush(list->_iterator_stack, as_literator(ft_memdup(&(t_literator){
			.current = list->current,
			.current_idx = list->current_idx,
			.method = list->_method
		},
		sizeof(t_literator)
	)));
	if (list->first)
		list->current = list->first->prev;
	else
		list->current = NULL;
	list->current_idx = (size_t)(-1);
	list->_method = lforward;
	return (list);
}
