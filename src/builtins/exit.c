/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:17 by paul              #+#    #+#             */
/*   Updated: 2024/07/11 21:51:36 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void	exit_cleanup(t_fds fds, long exit_status)
{
	if (fds.fd_in != -2 && fds.fd_in != 0)
		close(fds.fd_in);
	if (fds.fd_out != -2 && fds.fd_out != 1)
		close(fds.fd_out);
	if (fds.fd_err != -2 && fds.fd_err != 2)
		close(fds.fd_err);
	finish(true);
	exit((unsigned char)exit_status);
}

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
	exit_cleanup(fds, exit_status);
	return (1);
}
/* On failure, bash's exit builtin sets the exit status to 1 if there was no
 * previous command (new shell). Otherwise, the exit status is not altered.
 */
