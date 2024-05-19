#include "libft.h"

#include <stdbool.h>
#include <stdlib.h>

bool	gc_free(const char *ctx)
{
	t_list	*ptrs;

	(void)ctx;
	ptrs = gc_add(NULL);
	ldestroy(ptrs);
	return (true);
}
