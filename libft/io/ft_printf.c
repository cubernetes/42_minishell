/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 05:29:04 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 17:09:18 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>

int	ft_vprintf(const char *fmt, va_list ap)
{
	int	count;

	count = 0;
	while (*fmt)
	{
		if (*fmt != '%')
			count += ft_putchar(*fmt++);
		else
			count += ft_putfmt(&fmt, ap);
	}
	return (count);
}

int	ft_printf(const char *fmt, ...)
{
	int		count;
	va_list	ap;

	if (!fmt)
		return (-1);
	va_start(ap, fmt);
	count = ft_vprintf(fmt, ap);
	va_end(ap);
	return (count);
}
