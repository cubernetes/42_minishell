/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:05:31 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:05:31 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>

void	*ft_memmove(void *dest, void const *src, size_t n)
{
	char		*d;
	char const	*s;

	if (!src && !dest)
		return (0);
	d = dest;
	s = src;
	if (src >= dest)
		while (n--)
			*d++ = *s++;
	else
		while (n--)
			*(d + n) = *(s + n);
	return (dest);
}
