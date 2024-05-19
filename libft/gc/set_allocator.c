#include "libft.h"

#include <stdlib.h>

void	*(*set_allocator(void *(*_allocator)(size_t size)))(size_t size)
{
	static void	*(*allocator)(size_t size);

	if (_allocator != NULL)
		allocator = _allocator;
	if (allocator == NULL)
		allocator = malloc;
	return (allocator);
}
