/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_allocator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:31 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:04:31 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>

void	*(*set_allocator(void *(*_allocator)(size_t size)))(size_t size)
{
	static void	*(*allocator)(size_t size);

	if (_allocator != NULL)
		allocator = _allocator;
	if (allocator == NULL)
		allocator = malloc;
	return (allocator);
}
