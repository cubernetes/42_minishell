/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 09:35:19 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/01 12:16:17 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/memory.h"
#include "../../libft/libft.h"
#include <execinfo.h>
#include <stdlib.h>

#define MAX_CALLSTACK_SIZE 128

/* Assumes a format like this: */
/* /usr/lib/libc.so.6(__libc_start_main+0x8a) [0x7d810a29cd8a] */
void	print_stackframe(char *frame)
{
	while (*frame)
	{
		if (*frame == '(')
			(ft_putchar(*frame), ft_putstr("\033[41;30m"));
		else if (*frame == '+' || *frame == ')')
			(ft_putstr("\033[m"), ft_putchar(*frame));
		else
			ft_putchar(*frame);
		++frame;
	}
	ft_putchar('\n');
}

/* print current callstack, minus this function itself and minus 3 top-levels,
 * which are usually libc and _start
 */
void	print_callstack(void)
{
	void	*callstack[MAX_CALLSTACK_SIZE];
	int		frames;
	char	**symbols;

	frames = backtrace(callstack, MAX_CALLSTACK_SIZE);
	symbols = backtrace_symbols(callstack, frames);
	frames -= 3;
	while (--frames)
		print_stackframe(symbols[frames]);
	free(symbols);
}
