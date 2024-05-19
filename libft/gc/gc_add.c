#include "libft.h"

t_list	*gc_add(void *ptr)
{
	static t_list	*ptrs = NULL;

	if (ptrs == NULL)
		ptrs = lnew();
	return (lpush(ptrs, as_ptr(ptr)));
}
