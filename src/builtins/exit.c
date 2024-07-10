/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:17 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 20:14:15 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/* TODO: make unsigned char */
int	builtin_exit(char **argv, t_fds fds)
{
	long	exit_status;
	char	*name;
	int		status;

	(void)fds;
	name = *argv++;
	if (*argv == NULL)
		exit_status = ft_atoi(get_var("?")->value);
	else
	{
		exit_status = ft_atol_status(*argv, &status);
		if (status & 13)
			minishell_error(2, 1, "%s: %s: numeric argument required",
				name, *argv);
		if (argv[1] && minishell_error(1, 0,
				"%s: too many arguments", name))
		{
			exit_status = get_last_exit_status();
			if (exit_status)
				return ((unsigned char)exit_status);
			return (1);
		}
	}
	(finish(true), exit((unsigned char)exit_status));
	return (1);
}
/* On failure, bash's exit builtin sets the exit status to 1 if there was no
 * previous command (new shell). Otherwise, the exit status is not altered.
 */
