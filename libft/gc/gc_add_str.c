#include "libft.h"

char	*gc_add_str(void *ptr)
{
	return (gc_add(ptr)->last->as_str);
}
