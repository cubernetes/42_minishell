/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:07:58 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:08:02 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

int	ft_putnbr_fd(int nb, int fd)
{
	int	count;

	count = 0;
	if (nb > 9)
	{
		count += ft_putnbr_fd(nb / 10, fd);
		count += ft_putchar_fd((char)(nb % 10 + '0'), fd);
	}
	else if (nb == INT_MIN)
	{
		count += ft_putnbr_fd(nb / 10, fd);
		count += ft_putnbr_fd(-(nb % 10), fd);
	}
	else if (nb < 0)
	{
		count += ft_putchar_fd('-', fd);
		count += ft_putnbr_fd(-nb, fd);
	}
	else
		count += ft_putchar_fd((char)(nb % 10 + '0'), fd);
	return (count);
}
