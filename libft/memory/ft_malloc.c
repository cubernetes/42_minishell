/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 07:51:00 by tosuman           #+#    #+#             */
/*   Updated: 2024/04/11 21:26:02 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

t_bool	dont_free(void *data)
{
	(void)data;
	return (1);
}

t_bool	ft_free(void *ptr)
{
	/* ft_printf("\033[32mFREE\033[m\n"); */
	/* print_callstack(); */
	free(ptr);
	return (TRUE);
}

t_bool	free_all_ptrs(void)
{
	t_deque	*ptrs;
	t_deque	*static_ptrs;
	t_di	*di;

	static_ptrs = manage_static_ptr(NULL);
	ptrs = manage_ptr(NULL);
	di = di_begin(static_ptrs);
	while (di_next(di))
		*(void **)(di_get(di)->as_ptr) = NULL;
	ft_malloc_deque_free(static_ptrs, dont_free);
	ft_malloc_deque_free(ptrs, ft_free);
	return (TRUE);
}

t_deque	*manage_static_ptr(void **ptr)
{
	static t_deque	*static_ptrs = NULL;

	if (static_ptrs == NULL)
	{
		static_ptrs = ft_malloc_deque_init();
		ft_malloc_deque_push_ptr_right(static_ptrs, &static_ptrs);
	}
	if (ptr != NULL)
		ft_malloc_deque_push_ptr_right(static_ptrs, ptr);
	return (static_ptrs);
}

t_deque	*manage_ptr(void *ptr)
{
	static t_deque	*ptrs = NULL;

	if (ptrs == NULL && (manage_static_ptr((void **)&ptrs), 1))
		ptrs = ft_malloc_deque_init();
	ft_malloc_deque_push_ptr_right(ptrs, ptr);
	return (ptrs);
}

/* TODO: fix this: calling free_all_ptrs more than once causes SIGSEGV */
/* malloc wrapper that fails gracefully and frees memory when it can */
void	*ft_malloc(size_t size)
{
	void	*ptr;

	/* ft_printf("\033[31mMALLOC\033[m\n"); */
	/* print_callstack(); */
	ptr = malloc(size);
	if (!ptr)
	{
		ft_printf("\033[41;30mft_malloc: %s\033[m\n\033[31mCallstack "
			"(reverse):\033[m\n", strerror(errno));
		/* print_callstack(); */ /* preferrably keep this, but it contains forbidden functions... */
		exit(EXIT_FAILURE);
	}
	(void)manage_ptr(ptr);
	return (ptr);
}
