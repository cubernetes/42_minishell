#include "libft.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/* TODO: Rethink */
bool	ft_free(void *ptr)
{
	if (get_allocator() == malloc)
		free(ptr);
	else
		ft_dprintf(STDERR_FILENO, "Warning, called ft_free(), but garbage "
			"collection is active. Doing nothing.\n");
	return (true);
}
