/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:32 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:04:32 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>

t_list	*gc_add(void *ptr)
{
	t_list	*ptrs;
	void	*(*prev_allocator)(size_t);

	prev_allocator = get_allocator();
	set_allocator(malloc);
	ptrs = gc_get_context();
	(void)lpush(ptrs, as_gc_ptr(ptr));
	set_allocator(prev_allocator);
	return (ptrs);
}
