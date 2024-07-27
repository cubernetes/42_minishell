/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:20:52 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/11 20:12:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <unistd.h>
#include <stdlib.h>

char	*set_err_fmt(int flags, const char **fmt)
{
	char	*errfmt;

	errfmt = ft_strjoin(var_lookup("0"), ": ");
	if (!option_enabled('i'))
	{
		if (option_enabled('c') && flags & SYNTAX_ERROR)
			errfmt = ft_strjoin(errfmt, "-c: ");
		errfmt = ft_strjoin(errfmt, ft_strjoin("line ",
					ft_strjoin(var_lookup("LINENO"), ": ")));
	}
	if ((*fmt)[0] != '!')
		errfmt = ft_replace_all(errfmt, "%", "%%");
	else
	{
		errfmt = "";
		++(*fmt);
	}
	return (errfmt);
}

/*TODO: what happens in infinite loop in minishell with ctrl + c and cntr + d*/
int	minishell_error(int exit_code, int flags, const char *fmt, ...)
{
	va_list	ap;
	char	*errfmt;
	char	*err;

	if (option_enabled('i') && flags & DO_EXIT)
		ft_dprintf(STDERR_FILENO, "exit\n");
	va_start(ap, fmt);
	errfmt = set_err_fmt(flags, &fmt);
	err = ft_strjoin(errfmt, fmt);
	err = ft_strjoin(err, "\n");
	ft_vdprintf(STDERR_FILENO, err, &ap);
	va_end(ap);
	if (!option_enabled('i') && flags & SYNTAX_ERROR && fmt[0] != '!')
	{
		err = ft_strjoin(errfmt, "`%s'\n");
		ft_dprintf(STDERR_FILENO, err, var_lookup("CURRENT_LINE"));
	}
	if (flags & DO_EXIT)
	{
		finish(false);
		exit(exit_code);
	}
	return (exit_code);
}
