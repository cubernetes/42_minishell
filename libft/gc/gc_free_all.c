#include "libft.h"

#include <stdbool.h>

bool	gc_free_all(void)
{
	return (gc_ctx_manager(DESTROY_ALL_CONTEXTS, NULL), 1);
}
