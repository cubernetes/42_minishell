#include "libft.h"

t_list	*gc_get_context(void)
{
	return (gc_ctx_manager(GET_CONTEXT, NULL).ctx);
}
