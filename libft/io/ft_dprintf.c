/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 05:29:04 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 17:09:51 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>

int	ft_vdprintf(int fd, const char *fmt, va_list ap)
{
	int	count;

	count = 0;
	while (*fmt)
	{
		if (*fmt != '%')
			count += ft_putchar_fd(*fmt++, fd);
		else
			count += ft_putfmt_fd(fd, &fmt, ap);
	}
	return (count);
}

int	ft_dprintf(int fd, const char *fmt, ...)
{
	int		count;
	va_list	ap;

	if (!fmt)
		return (-1);
	va_start(ap, fmt);
	count = ft_vdprintf(fd, fmt, ap);
	va_end(ap);
	return (count);
}
