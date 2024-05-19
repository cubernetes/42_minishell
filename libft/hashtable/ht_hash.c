#include "libft.h"

#include <stdint.h>

static uint64_t	fnv_1a_64(char *input_str)
{
	unsigned char	*str;
	uint64_t		hash;

	str = (unsigned char *)input_str;
	hash = OFFSET_BASIS_64;
	while (*str)
	{
		hash ^= (unsigned long)*str;
		hash *= FNV_PRIME_64;
		str++;
	}
	return (hash);
}

uint64_t	ht_hash(char *key)
{
	return (fnv_1a_64(key));
}
