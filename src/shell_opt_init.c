/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_opt_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:18:25 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 21:19:34 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <unistd.h>
#include <stdlib.h>

t_list	*shell_opt_init(char *const argv[], bool *implicit_s,
	char **opts, int *is_login_shell)
{
	t_list	*options;
	char	erropt;

	*implicit_s = false;
	options = liter(ft_getopt_plus(argv, "acefilnstuvxC", &erropt, &optind));
	if (erropt)
	{
		(void)minishell_error(2, 0, "-%c: invalid option", erropt);
		ft_dprintf(STDERR_FILENO, "Usage:\t%s [option] ...\n\t%s [option] "
			"script-file ...\nShell options:\n\t-ils or -c command "
			"(invocation only)\n\t-aefnstuvxC\n", MINISHELL_NAME,
			MINISHELL_NAME);
		finish(false);
		exit(2);
	}
	*opts = "";
	*is_login_shell = false;
	return (options);
}
