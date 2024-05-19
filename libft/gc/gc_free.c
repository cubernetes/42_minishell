#include "libft.h"

#include <stdbool.h>
#include <stdlib.h>

bool	gc_free(char *ctx)
{
	gc_ctx_manager(DESTROY_CONTEXT, ctx);
	return (true);
}
