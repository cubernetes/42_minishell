/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:08:47 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/16 17:35:53 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>
#include <unistd.h>

char	*get_next_line(int fd)
{
	static t_gnl_vars	x;

	while (x.prv[fd] && x.prv[fd][x.len])
		++x.len;
	x.buf = malloc((size_t)x.len + BUFFER_SIZE + 1);
	x.b = read(fd, x.buf + x.len, BUFFER_SIZE);
	if (x.b < 0)
		return (free(x.buf), free(x.prv[fd]), x.len = 0, x.prv[fd] = NULL);
	(free(NULL), x.buf[x.len + x.b] = 0, x.i = x.len, x.j = -1);
	while (--x.len >= 0)
		x.buf[x.len] = x.prv[fd][x.len];
	while (x.buf[++x.len] && x.buf[x.len] != '\n')
		;
	if (!x.buf[x.len] && x.b == 0 && (free(x.buf), 1))
		return (x.buf = x.prv[fd], x.prv[fd] = NULL, x.len = 0, x.buf);
	if (!x.buf[x.len] && x.b == BUFFER_SIZE && (free(x.prv[fd]), 1))
		return (x.prv[fd] = x.buf, x.len = 0, get_next_line(fd));
	if (!x.buf[x.len] || (x.buf[x.len] == '\n' && !x.buf[x.len + 1]))
		return (free(x.prv[fd]), x.prv[fd] = NULL, x.len = 0, x.buf);
	(free(x.prv[fd]), x.prv[fd] = malloc(sizeof(char) * (size_t)(x.i + ++x.b)));
	if (!x.prv[fd])
		return (free(x.buf), free(x.prv[fd]), x.len = 0, x.prv[fd] = NULL);
	while (x.buf[x.len + 1 + ++x.j])
		x.prv[fd][x.j] = x.buf[x.len + 1 + x.j];
	return (x.prv[fd][x.j] = 0, x.buf[x.len + 1] = 0, x.len = 0, x.buf);
}
