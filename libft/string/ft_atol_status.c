/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:08:37 by tischmid          #+#    #+#             */
/*   Updated: 2024/06/10 22:13:19 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	parse_tail(char const *nptr, int *status)
{
	if (*nptr != '\0')
		*status = *status | 2;
	while (*nptr)
	{
		if (*nptr != ' ' && *nptr != '\n' && *nptr != '\t')
			*status = *status | 4;
		++nptr;
	}
}

static long	ft_atol_status_neg(char const *nptr, int *status)
{
	long	res;
	long	prev_res;

	res = 0;
	prev_res = 0;
	while (ft_isdigit(*nptr))
	{
		res = res * 10 - (*nptr - '0');
		if (res > prev_res)
			*status = *status | 1;
		prev_res = res;
		++nptr;
	}
	parse_tail(nptr, status);
	return (res);
}

static long	ft_atol_status_pos(char const *nptr, int *status)
{
	long	res;
	long	prev_res;

	res = 0;
	prev_res = 0;
	while (ft_isdigit(*nptr))
	{
		res = res * 10 + (*nptr - '0');
		if (res < prev_res)
			*status = *status | 1;
		prev_res = res;
		++nptr;
	}
	parse_tail(nptr, status);
	return (res);
}

/* status == 0 -> all good */
/* status == 1 -> overflow */
/* status == 2 -> doesn't end with nul */
/* status == 3 -> doesn't end with only whitespace */
long ft_atol_status(char const *nptr, int *status)
{
	*status = 0;
	while (ft_isspace(*nptr))
		++nptr;
	if (*nptr == '-')
	{
		++nptr;
		return (ft_atol_status_neg(nptr, status));
	}
	return (ft_atol_status_pos(nptr, status));
}
