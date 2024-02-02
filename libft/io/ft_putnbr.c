/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:08:16 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:08:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <limits.h>

int	ft_putnbr(int nb)
{
	int	count;

	count = 0;
	if (nb > 9)
	{
		count += ft_putnbr(nb / 10);
		count += ft_putchar((char)(nb % 10 + '0'));
	}
	else if (nb == INT_MIN)
	{
		count += ft_putnbr(nb / 10);
		count += ft_putnbr(-(nb % 10));
	}
	else if (nb < 0)
	{
		count += ft_putchar('-');
		count += ft_putnbr(-nb);
	}
	else
		count += ft_putchar((char)(nb % 10 + '0'));
	return (count);
}
