#include "libft.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

/* TODO: fix this: calling gc_free more than once causes SIGSEGV */
/* malloc wrapper that fails gracefully and frees memory when it can */
void	*gc_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_printf("\033[41;30mft_malloc: %s\033[m\n\033[31mCallstack "
			"(reverse):\033[m\n", strerror(errno));
		print_callstack();
		exit(EXIT_FAILURE);
	}
	(void)gc_add(ptr);
	return (ptr);
}
