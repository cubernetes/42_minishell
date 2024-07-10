/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:30 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 16:03:59 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	builtin_shift(char **argv, t_fds fds) // TODO: use fds
{
	int	num_args;
	int	idx;

	(void)argv;
	(void)fds;
	num_args = ft_atoi(var_lookup("#"));
	idx = 1;
	while (idx <= num_args)
	{
		set_var(ft_itoa(idx), var_lookup(ft_itoa(idx + 1)),
			get_flags(ft_itoa(idx)));
		++idx;
	}
	return (0);
}
