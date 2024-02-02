/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 07:51:00 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/02 02:22:14 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "./memory.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_bool	ft_free(void *ptr)
{
	free(ptr);
	return (TRUE);
}

t_bool	free_all_ptrs(void)
{
	t_ddeque	*ptrs;

	ptrs = manage_ptrs(NULL);
	ft_malloc_ddeque_free(ptrs, ft_free);
	return (TRUE);
}

/* globals are not allowed? Well, oopsie */
t_ddeque	*manage_ptrs(void *ptr)
{
	static t_ddeque	*ptrs = NULL;

	if (!ptrs)
		ptrs = ft_malloc_ddeque_init();
	ft_malloc_ddeque_push_value_bottom(ptrs, ptr);
	return (ptrs);
}

/* malloc wrapper that fails gracefully and frees memory when it can */
void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_printf("\033[41;30mft_malloc: %s\033[m\n\033[31mCallstack "
			"(reverse):\033[m\n", strerror(errno));
		print_callstack();
		exit(EXIT_FAILURE);
	}
	(void)manage_ptrs(ptr);
	return (ptr);
}
