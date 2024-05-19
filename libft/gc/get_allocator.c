#include "libft.h"

#include <stddef.h>

void	*(*get_allocator(void))(size_t size)
{
	return (set_allocator(NULL));
}
