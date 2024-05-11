#include "libft.h"

/** Return the next available line including the newline character (if
 *  available) from a file descriptor.
 *
 *  @param fd: the file descriptor to read from
 *  @returns: a single line, potentially including a newline character (0x0a)
 */
char	*get_next_line(int fd)
{
	ssize_t	bytes_read;
	char	c;
	t_list	*chars;
	char	*str;

	chars = new_list();
	bytes_read = read(fd, &c, 1);
	while (bytes_read)
	{
		if (c == '\n')
			break ;
		push(chars, as_char(c));
		bytes_read = read(fd, &c, 1);
	}
	if (bytes_read == 0 && chars->len == 0)
		return (NULL);
	str = ft_malloc(sizeof(*str) * (chars->len + 1));
	str[chars->len] = 0;
	start_iterator(chars);
	while (next(chars))
		str[chars->current_idx] = chars->current->as_char;
	return (str);
}
/* this implementation reads character by character */
