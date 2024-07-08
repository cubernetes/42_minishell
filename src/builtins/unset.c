/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:37 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:32:38 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <unistd.h>

int	builtin_unset(char **argv, t_fds fds)
{
	int		exit_status;
	char	*name;
	char	erropt;

	(void)fds;
	ft_getopt(argv, "", &erropt, &optind);
	name = *argv++;
	if (erropt)
		return (minishell_error(2, true, false,
				"%s: -%c: invalid option\n%s: usage: unset [name ...]",
				name, erropt, name));
	exit_status = 0;
	while (*argv)
	{
		if (!unset_var(*argv))
			exit_status = minishell_error(1, false, false,
					"%s: %s: cannot unset: readonly variable",
					name, *argv);
		++argv;
	}
	return (exit_status);
}
