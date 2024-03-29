/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 07:51:00 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/27 17:57:38 by tosuman          ###   ########.fr       */
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
	t_deque	*ptrs;

	ptrs = manage_ptrs(NULL);
	ft_malloc_deque_free(ptrs, ft_free);
	return (TRUE);
}

/* globals are not allowed? Well, oopsie */
t_deque	*manage_ptrs(void *ptr)
{
	static t_deque	*ptrs = NULL;

	if (!ptrs)
		ptrs = ft_malloc_deque_init();
	ft_malloc_deque_push_ptr_right(ptrs, ptr);
	return (ptrs);
}

/* TODO: fix this: calling free_all_ptrs more than once causes SIGSEGV */
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
