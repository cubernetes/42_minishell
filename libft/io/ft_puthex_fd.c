/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:50:49 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 23:09:52 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_puthex_fd(unsigned long nbr, int uppercase, int fd)
{
	int	digit;
	int	count;

	count = 0;
	if (nbr < 16)
	{
		if (uppercase)
			count += ft_putchar_fd(UHEX_DIGITS[nbr], fd);
		else
			count += ft_putchar_fd(HEX_DIGITS[nbr], fd);
		return (count);
	}
	if (uppercase)
		digit = UHEX_DIGITS[nbr % 16];
	else
		digit = HEX_DIGITS[nbr % 16];
	nbr /= 16;
	count += ft_puthex_fd(nbr, uppercase, fd);
	count += ft_putchar_fd((char)digit, fd);
	return (count);
}
