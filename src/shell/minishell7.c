/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell7.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:25:29 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 23:46:36 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void	parse_opts(t_list *options, char **opts, int *is_login_shell)
{
	while (lnext(options))
	{
		if ((char)options->current->as_getopt_arg == 'c')
		{
			if (!ft_strchr(*opts, 'c'))
				*opts = ft_strjoin(*opts, "c");
		}
		else if ((char)options->current->as_getopt_arg == 's')
		{
			if (!ft_strchr(*opts, 's'))
				*opts = ft_strjoin(*opts, "s");
		}
		else if (options->current->as_getopt_arg & 1 << 8 && ft_strchr(*opts,
				(char)options->current->as_getopt_arg))
			*opts = ljoin(lsplit(*opts, ft_strndup(&(char){(char)options
							->current->as_getopt_arg}, 1)), "");
		else if (!ft_strchr(*opts, (char)options->current->as_getopt_arg)
			&& (char)options->current->as_getopt_arg != 'l')
			*opts = ft_strjoin(*opts, ft_strndup(&(char){(char)options
						->current->as_getopt_arg}, 1));
		else if ((char)options->current->as_getopt_arg == 'l')
			*is_login_shell = true;
	}
}

void	setup_interactive_mode(char *opts)
{
	int		tty;

	if (ft_strchr(opts, 'i'))
	{
		tty = open("/dev/tty", O_WRONLY);
		if (tty == -1)
			(void)minishell_error(1, 0, "/dev/tty: ", strerror(errno));
		else
		{
			dup2(tty, STDERR_FILENO);
			close(tty);
		}
	}
}

void	setup_c_mode(char **opts, bool *implicit_s)
{
	if (!ft_strchr(*opts, 'c'))
	{
		if (!ft_strchr(*opts, 's'))
		{
			*opts = ft_strjoin(*opts, "s");
			*implicit_s = true;
		}
		if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
			if (!ft_strchr(*opts, 'i'))
				*opts = ft_strjoin(*opts, "i");
	}
}

void	set_implicit_interactive_mode(char *const argv[], char *opts,
	int implicit_s)
{
	int		i;

	if (ft_strchr(opts, 'c') || (ft_strchr(opts, 's') && !implicit_s))
	{
		i = 1;
		if (ft_strchr(opts, 'c'))
			i = 0;
		while (*argv)
		{
			set_var(ft_itoa(i), *argv, (t_flags){.special = true});
			++argv;
			++i;
		}
		set_var("#", ft_itoa(i - 1), (t_flags){.special = true});
	}
	else if (*argv != NULL)
	{
		ft_printf("Not implemented yet...\n");
	}
}
// run/source script or smth

// default
// (none)
// default -i
// i
// default interactive (i cannot be removed)
// is (adds i and s)
// default non-interactive
// s (adds s)
// default -c
// c (adds c)
// default non-interactive with -i
// is (adds i and s)
// default -c with -i
// ic (adds i and c)
//
// + - doesn't matter (cannot be negated)
// cs
// + - does matter, last one counts
// aefintuvxC
int	set_shell_options(char *const argv[])
{
	t_list	*options;
	char	*opts;
	bool	implicit_s;
	int		is_login_shell;

	options = liter(shell_opt_init(argv, &implicit_s, &opts, &is_login_shell));
	parse_opts(options, &opts, &is_login_shell);
	setup_interactive_mode(opts);
	setup_c_mode(&opts, &implicit_s);
	set_var("-", opts, (t_flags){.special = true});
	set_var("0", argv[0], (t_flags){.special = true});
	is_login_shell |= argv[0][0] == '-';
	argv += optind;
	if (ft_strchr(opts, 'c'))
	{
		if (*argv == NULL)
			minishell_error(2, 1, "-c: option requires an argument");
		set_var("MINISHELL_EXECUTION_STRING", *argv, (t_flags){0});
		++argv;
	}
	set_implicit_interactive_mode(argv, opts, implicit_s);
	return (is_login_shell);
}
