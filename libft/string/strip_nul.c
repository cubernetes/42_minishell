/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_nul.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:03:03 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:03:05 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stddef.h>

char	*strip_nul(char	*bytes, size_t size)
{
	size_t	num_nul;
	size_t	idx;
	size_t	idx2;
	char	*new_bytes;

	num_nul = 0;
	idx = (size_t)-1;
	while (++idx < size)
		if (bytes[idx] == '\0')
			++num_nul;
	new_bytes = ft_malloc(sizeof(*new_bytes) * (size - num_nul + 1));
	new_bytes[size - num_nul] = '\0';
	idx = (size_t)-1;
	idx2 = (size_t)-1;
	while (++idx < size)
		if (bytes[idx] != '\0')
			new_bytes[++idx2] = bytes[idx];
	return (new_bytes);
}
