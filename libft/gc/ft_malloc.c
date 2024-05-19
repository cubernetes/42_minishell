#include "libft.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = (get_allocator())(size);
	if (!ptr)
	{
		ft_printf("\033[41;30mft_malloc: %s\033[m\n\033[31mCallstack "
			"(reverse):\033[m\n", strerror(errno));
		print_callstack();
		exit(EXIT_FAILURE);
	}
	return (ptr);
}
