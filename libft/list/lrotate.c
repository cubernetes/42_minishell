#include "libft.h"

/* TODO: Document */
void	lrotate(t_list *list, int n)
{
	if (n > 0)
	{
		while (n--)
		{
			list->first = list->first->next;
			list->last = list->last->next;
		}
	}
	else if (n < 0)
	{
		while (n++)
		{
			list->first = list->first->prev;
			list->last = list->last->prev;
		}
	}
}
