/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 10:26:52 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/31 14:59:52 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <unistd.h>
#include <stdlib.h>
#define WIDTH 16
#define DELIMITER " "

static void	print_padded_hex(unsigned long addr_number, int length)
{
	int		digit;

	if (addr_number < 16)
	{
		while (--length > 0)
			ft_putchar('0');
		ft_putchar(HEX_DIGITS[addr_number]);
		return ;
	}
	digit = HEX_DIGITS[addr_number % 16];
	print_padded_hex(addr_number /= 16, length - 1);
	ft_putchar((char)digit);
}

static void	print_addr_vals(void *ptr, size_t width, int hex, char *delim)
{
	size_t			i;
	unsigned char	*addr;

	addr = (unsigned char *)ptr;
	i = 0;
	while (i < width)
	{
		if (hex)
			print_padded_hex(*addr, 2);
		else if (*addr > 31 && *addr < 127)
			ft_putchar(*(char *)addr);
		else
			ft_putchar('.');
		if ((!hex || (hex && i % 2 == 1)) && *delim)
			ft_putchar(*delim);
		++addr;
		++i;
	}
}

void	*ft_print_memory(void *ptr, size_t size)
{
	size_t	lines;
	size_t	width;
	size_t	missing_chars;
	char	*addr;

	(free(NULL), lines = 0, width = WIDTH);
	addr = (char *)ptr;
	while (++lines, 1)
	{
		if (size - (lines) * WIDTH < 16)
			width = size - lines * WIDTH;
		if (!width)
			break ;
		print_padded_hex((unsigned long) addr, 16);
		(ft_putstr(":" DELIMITER), print_addr_vals(addr, width, 1, DELIMITER));
		missing_chars = (WIDTH - width) * (2 + ft_strlen(DELIMITER));
		missing_chars -= (WIDTH - width) / 2;
		while (missing_chars-- > 0)
			ft_putchar(*DELIMITER);
		print_addr_vals((addr += width) - width, width, 0, "");
		ft_putchar('\n');
		if (width != WIDTH)
			break ;
	}
	return (addr);
}
