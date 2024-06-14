#include "libft.h"
#include <unistd.h>

t_fatptr	get_next_fat_line(int fd)
{
	ssize_t		bytes_read;
	char		c;
	t_list		*chars;
	t_fatptr	line;

	bytes_read = read(fd, &c, 1);
	if (bytes_read < 0)
		return ((t_fatptr){0});
	chars = lnew();
	while (bytes_read)
	{
		lpush(chars, as_char(c));
		if (c == '\n')
			break ;
		bytes_read = read(fd, &c, 1);
	}
	if (bytes_read == 0 && chars->len == 0)
		return ((t_fatptr){0});
	line.data = ft_malloc(sizeof(char) * (chars->len + 1));
	line.size = chars->len + 1;
	((char *)line.data)[chars->len] = 0;
	liter(chars);
	while (lnext(chars))
		((char *)line.data)[chars->current_idx] = chars->current->as_char;
	return (line);
}
/* this implementation reads character by character */
/* TODO: Test rigorously */
