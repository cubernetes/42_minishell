#include "libft.h"

#include <stdbool.h>
#include <stdlib.h>

bool	gc_free(const char *ctx)
{
	t_list	*ptrs;

	(void)ctx;
	ptrs = gc_add(NULL);
	set_allocator(malloc);
	ldestroy(ptrs);
	set_allocator(gc_malloc);
	return (true);
}
