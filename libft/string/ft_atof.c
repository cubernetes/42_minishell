/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:03:52 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:03:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_atof(const char *s)
{
	int		sign;
	double	number;
	double	power;

	number = 0.0;
	power = 1.0;
	while (ft_isspace(*s))
		++s;
	sign = (('-' != *s) - 1) * 2 + 1;
	if ('-' == *s || '+' == *s)
		++s;
	while (ft_isdigit(*s))
	{
		number = 10.0 * number + (*s - '0');
		++s;
	}
	if (*s == '.')
		++s;
	while (ft_isdigit(*s))
	{
		number = 10.0 * number + (*s - '0');
		power *= 10.0;
		++s;
	}
	return (sign * number / power);
}
