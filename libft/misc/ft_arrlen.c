#include <stddef.h>

size_t	ft_arrlen(char **strv)
{
	size_t	n;

	n = 0;
	while (*strv++)
		++n;
	return (n);
}
