/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_n_clr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 06:53:13 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:06:44 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_n_clr(const char *str, int n, bool color)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		if (color)
		{
			ft_putstr("\033[");
			ft_putnbr((i % 7) + 31);
			ft_putstr("m");
		}
		ft_printf("%s", str);
		if (color)
			ft_putstr("\033[m");
	}
}
