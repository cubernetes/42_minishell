#include "libft.h"

/** Push a data union to the end of a list.
 *
 *  @param list: the list to push `data' to
 *  @param data: the data union to push to `list'
 *  @returns: `list'
 */
t_list	*lpush(t_list list[static 1], t_data data)
{
	struct s_list_node	*node;

	node = ft_malloc(sizeof(*node));
	ft_memmove(node, &data, sizeof(data));
	if (list->len == 0)
	{
		list->first = node;
		list->current = node;
		list->last = node;
	}
	else
	{
		list->last->next = node;
		list->first->prev = node;
		if (list->first == list->current)
			list->current->prev = node;
	}
	node->next = list->first;
	node->prev = list->last;
	list->last = node;
	list->len++;
	return (list);
}
