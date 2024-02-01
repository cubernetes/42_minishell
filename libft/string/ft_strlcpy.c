/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:06:28 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:06:28 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcpy(char *dst, char const *src, size_t size)
{
	size_t	len;

	len = 0;
	while (len + 1 < size && src[len])
		*dst++ = src[len++];
	if (size)
		*dst = 0;
	while (src[len])
		len++;
	return (len);
}
