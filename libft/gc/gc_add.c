#include "libft.h"

#include <stdlib.h>

t_list	*gc_add(void *ptr)
{
	static t_list	*ptrs = NULL;

	set_allocator(malloc);
	if (ptrs == NULL)
		ptrs = lnew();
	lpush(ptrs, as_gc_ptr(ptr));
	set_allocator(gc_malloc);
	return (ptrs);
}
