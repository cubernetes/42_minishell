#include "libft.h"
#include "libft.h"

/** Push a data union to the beginning of a list ("prepend").
 *
 *  @param list: the list to prepend `data' to
 *  @param data: the data union to prepend to `list'
 *  @returns: `list'
 */
t_list	*lpush_left(t_list list[static 1], t_data data)
{
	struct s_list_node	*node;

	node = ft_malloc(sizeof(*node));
	ft_memmove(node, &data, sizeof(data));
	if (list->len == 0)
	{
		list->first = node;
		list->current = NULL;
		list->last = node;
	}
	else
	{
		list->first->prev = node;
		list->last->next = node;
	}
	node->next = list->first;
	node->prev = list->last;
	list->first = node;
	list->len++;
	return (list);
}
/* TODO: Test rigorously */
