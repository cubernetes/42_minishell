#include "libft.h"

#include <stdbool.h>

/* TODO: Document */
static void	highlight_elements(
	t_list_node	*current,
	t_list_node	*current_first,
	t_list_node	*current_current,
	t_list_node	*current_last)
{
	if (current == current_first)
		ft_printf("\033[32m");
	if (current == current_current)
		ft_printf("\033[4m");
	if (current == current_last)
		ft_printf("\033[2m");
}

/* TODO: Document */
void	lprint(t_list *list, void (print)(t_data data, int n))
{
	t_list_node	*current_first;
	t_list_node	*current_current;
	t_list_node	*current_last;

	current_first = list->first;
	current_current = list->current;
	current_last = list->last;
	liter(list);
	while (lnext(list))
	{
		highlight_elements(list->current, current_first, current_current,
			current_last);
		print(as_data(list->current), (int)list->current_idx);
		if (list->current == current_first
				|| list->current == current_current
				|| list->current == current_last)
			ft_printf("\033[m");
	}
	if (list->first == NULL)
		ft_printf(EMPTY_LIST);
	ft_printf("\n");
}
