/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:07:39 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/25 22:17:24 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_itoa(int n)
{
	char			*str;
	size_t			n_len;
	unsigned int	n_cpy;
	int				is_neg;

	is_neg = n < 0;
	n_len = 0;
	n_cpy = ft_abs(n) + !n;
	while (n_cpy * ++n_len != 0)
		n_cpy /= 10;
	--n_len;
	str = malloc(sizeof(*str) * (n_len + (size_t)is_neg + 1));
	if (!str)
		return (NULL);
	str += (int)n_len + is_neg;
	n_cpy = ft_abs(n);
	*str-- = 0;
	while (n_len--)
	{
		*str-- = (char)(n_cpy % 10 + '0');
		n_cpy /= 10;
	}
	if (is_neg)
		*str-- = '-';
	return (++str);
}
