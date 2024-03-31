/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:08:47 by tischmid          #+#    #+#             */
/*   Updated: 2024/04/01 00:18:28 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>
#include <unistd.h>

/* might misbehave when using manage_static_ptrs() and manage_ptrs() */
char	*get_next_line(int fd)
{
	static t_gnl_vars	x;

	while (x.prv[fd] && x.prv[fd][x.len])
		++x.len;
	x.buf = ft_malloc((size_t)x.len + GNL_BUFFER_SIZE + 1);
	x.b = read(fd, x.buf + x.len, GNL_BUFFER_SIZE);
	if (x.b < 0)
		return (x.len = 0, x.prv[fd] = NULL);
	(free(NULL), x.buf[x.len + x.b] = 0, x.i = x.len, x.j = -1);
	while (--x.len >= 0)
		x.buf[x.len] = x.prv[fd][x.len];
	while (x.buf[++x.len] && x.buf[x.len] != '\n')
		;
	if (!x.buf[x.len] && x.b == 0)
		return (x.buf = x.prv[fd], x.prv[fd] = NULL, x.len = 0, x.buf);
	if (!x.buf[x.len] && x.b == GNL_BUFFER_SIZE)
		return (x.prv[fd] = x.buf, x.len = 0, get_next_line(fd));
	if (!x.buf[x.len] || (x.buf[x.len] == '\n' && !x.buf[x.len + 1]))
		return (x.prv[fd] = NULL, x.len = 0, x.buf);
	(free(NULL), x.prv[fd] = ft_malloc(sizeof(char) * (size_t)(x.i + ++x.b)));
	if (!x.prv[fd])
		return (x.len = 0, x.prv[fd] = NULL);
	while (x.buf[x.len + 1 + ++x.j])
		x.prv[fd][x.j] = x.buf[x.len + 1 + x.j];
	return (x.prv[fd][x.j] = 0, x.buf[x.len + 1] = 0, x.len = 0, x.buf);
}
