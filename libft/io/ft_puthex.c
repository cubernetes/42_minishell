/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:07:31 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:07:31 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_puthex(unsigned long nbr, int uppercase)
{
	int	digit;
	int	count;

	count = 0;
	if (nbr < 16)
	{
		if (uppercase)
			count += ft_putchar(UHEX_DIGITS[nbr]);
		else
			count += ft_putchar(HEX_DIGITS[nbr]);
		return (count);
	}
	if (uppercase)
		digit = UHEX_DIGITS[nbr % 16];
	else
		digit = HEX_DIGITS[nbr % 16];
	nbr /= 16;
	count += ft_puthex(nbr, uppercase);
	count += ft_putchar((char)digit);
	return (count);
}
