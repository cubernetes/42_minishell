/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putfmt_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 17:07:04 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 23:09:35 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putfmt_fd(int fd, const char **fmt, va_list *ap)
{
	int	c;

	if (*(++*fmt) == 'c')
		c = ft_putchar_fd((char)va_arg(*ap, int), fd);
	else if (**fmt == 'd' || **fmt == 'i')
		c = ft_putnbr_fd((int)va_arg(*ap, int), fd);
	else if (**fmt == 'u')
		c = ft_putnbr_fd((int)va_arg(*ap, unsigned int), fd);
	else if (**fmt == 'x')
		c = ft_puthex_fd((unsigned long)va_arg(*ap, unsigned int), 0, fd);
	else if (**fmt == 'X')
		c = ft_puthex_fd((unsigned long)va_arg(*ap, unsigned int), 1, fd);
	else if (**fmt == 's')
		c = ft_putstr_fd(va_arg(*ap, char *), fd);
	else if (**fmt == 'p')
		c = ft_putptr_fd(va_arg(*ap, void *), fd);
	else if (**fmt == '%')
		c = ft_putchar_fd('%', fd);
	else
		c = ft_dprintf(fd, "%%%c", **fmt);
	++*fmt;
	return (c);
}
