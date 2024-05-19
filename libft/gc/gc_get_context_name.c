#include "libft.h"

char	*gc_get_context_name(void)
{
	return (gc_ctx_manager(GET_CONTEXT, NULL).ctx_name);
}
