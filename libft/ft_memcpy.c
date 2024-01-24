/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:05:24 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:05:25 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, void const *src, size_t n)
{
	char		*d;
	char const	*s;

	if (!src && !dest)
		return (0);
	d = dest;
	s = src;
	while (n--)
		*d++ = *s++;
	return (dest);
}
