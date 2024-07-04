/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:35 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:06:01 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/* TODO: Rethink */
bool	ft_free(void *ptr)
{
	if (get_allocator() == malloc)
		free(ptr);
	else
	{
	}
	return (true);
}
	/* ft_dprintf(STDERR_FILENO, "Warning, called ft_free(), but garbage " */
		/* "collection is active. Doing nothing.\n"); */
