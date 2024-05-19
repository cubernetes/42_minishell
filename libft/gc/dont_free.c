#include <stdbool.h>

bool	dont_free(void *ptr)
{
	(void)ptr;
	return (true);
}
