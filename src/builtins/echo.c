/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:11 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:32:12 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <unistd.h>
#include <stdbool.h>

int	builtin_echo(char **argv, t_fds fds)
{
	bool	newline;
	char	erropt;
	t_list	*opts;

	opts = liter(ft_getopt(argv, "n", &erropt, &optind));
	if (!ft_strcmp(argv[optind - 1], "--"))
		--optind;
	newline = true;
	if (opts->len > 0)
		newline = false;
	while (argv[optind] != NULL)
	{
		ft_dprintf(fds.fd_out, "%s", argv[optind++]);
		if (argv[optind] != NULL)
			ft_dprintf(fds.fd_out, " ");
	}
	if (newline)
		ft_dprintf(fds.fd_out, "\n");
	return (0);
}
