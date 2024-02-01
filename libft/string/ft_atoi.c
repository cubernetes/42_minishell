/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:08:37 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:08:37 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_atoi(char const *nptr)
{
	int	res;
	int	sign;

	while (ft_isspace(*nptr++))
		;
	sign = 1;
	if (*--nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			sign = -1;
	res = 0;
	while (ft_isdigit(*nptr))
		res = res * 10 + *nptr++ - '0';
	return (sign * res);
}
