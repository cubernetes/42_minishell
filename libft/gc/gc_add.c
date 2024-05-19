#include "libft.h"

#include <stdlib.h>

t_list	*gc_add(void *ptr)
{
	t_list	*ptrs;
	void	*(*prev_allocator)(size_t);

	prev_allocator = get_allocator();
	set_allocator(malloc);
	ptrs = lpush(gc_get_context(), as_gc_ptr(ptr));
	set_allocator(prev_allocator);
	return (ptrs);
}
