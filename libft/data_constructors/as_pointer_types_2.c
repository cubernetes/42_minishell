#include "libft.h"

t_data	as_var(t_var *as_var)
{
	return ((t_data){.as_var = as_var, .type = TYPE_VAR});
}

t_data	as_gc_ptr(t_var *as_gc_ptr)
{
	return ((t_data){.as_ptr = as_gc_ptr, .type = TYPE_GC_PTR});
}
