/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:24 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 20:09:52 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

int	builtin_pwd(char **argv, t_fds fds)
{
	char	*cwd;
	char	*name;

	name = *argv++;
	cwd = get_saved_cwd();
	if (cwd[0] != '\0')
	{
		ft_dprintf(fds.fd_out, "%s\n", cwd);
		return (0);
	}
	else
	{
		cwd = gc_add_str(getcwd(NULL, 0));
		if (cwd == NULL)
		{
			minishell_error(0, 0, "!%s: error retrieving current "
				"directory: getcwd: cannot access parent directory: %s",
				name, strerror(errno));
			return (1);
		}
		else
			return (ft_dprintf(fds.fd_out, "%s\n", cwd), 0);
	}
}
