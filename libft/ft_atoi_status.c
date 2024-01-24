/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:28:18 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/15 23:24:47 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

int	ft_isspace(int c);
int	ft_isdigit(int c);

int	ft_atoi_status(char const *nptr, int *status)
{
	long	res;
	int		sign;

	sign = 1;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			sign = -1;
	res = 0;
	if (!*nptr)
		*status = -1;
	while (ft_isdigit(*nptr))
	{
		res = res * 10 + *nptr++ - '0';
		if ((sign == 1 && res > INT_MAX)
			|| (sign == -1 && res > -(long)INT_MIN))
			*status = -1;
	}
	if (*nptr)
		*status = -1;
	return ((int)(sign * res));
}
