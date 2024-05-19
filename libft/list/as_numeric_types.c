#include "list.h"

/** Return a data union initialized with the int argument.
 *
 *  @param as_int: the integer member of the union
 *  @returns: the data union containing `as_int'
 */
t_data	as_int(int as_int)
{
	return ((t_data){.as_int = as_int, .type = TYPE_INT});
}

/** Return a data union initialized with the size_t argument.
 *
 *  @param as_size_t: the size_t member of the union
 *  @returns: the data union containing `as_size_t'
 */
t_data	as_size_t(size_t as_size_t)
{
	return ((t_data){.as_size_t = as_size_t, .type = TYPE_SIZE_T});
}

/** Return a data union initialized with the char argument.
 *
 *  @param as_char: the char member of the union
 *  @returns: the data union containing `as_char'
 */
t_data	as_char(char as_char)
{
	return ((t_data){.as_char = as_char, .type = TYPE_CHAR});
}
