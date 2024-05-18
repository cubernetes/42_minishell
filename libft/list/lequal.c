#include "list.h"
#include <stdbool.h>

/* TODO: Document */
/* cmp must return 0 if comparison fails */
/* TODO: Not required: Implemented variadic di_next() function */
bool	lequal(t_list *list_a, t_list *list_b,
		bool (cmp)(t_data data1, t_data data2))
{
	t_list_node	*a;
	t_list_node	*b;

	a = lnext(liter(list_a));
	b = lnext(liter(list_b));
	while (a && b)
	{
		if (!cmp(as_data(list_a->current), as_data(list_b->current)))
			return (llast(list_a), llast(list_b), false);
		a = lnext(list_a);
		b = lnext(list_b);
	}
	if (a == NULL && b == NULL)
		return (true);
	if (a)
		llast(list_a);
	if (b)
		llast(list_b);
	return (false);
}
/* TODO: Test rigorously */
