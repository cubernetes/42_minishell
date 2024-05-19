#include "libft.h"

t_list	*gc_set_context(char *new_ctx_name)
{
	return (gc_ctx_manager(SET_CONTEXT, new_ctx_name).ctx);
}
