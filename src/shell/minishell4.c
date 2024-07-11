/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:04:12 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 20:09:20 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <stddef.h>
#include <stdio.h>             /* needed for FILE typedef needed by readline */
#include <readline/history.h>  /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h>            /* exit(), DONT USE free or malloc! */
#include <string.h>
#include <unistd.h>            /* STDERR_FILENO */
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

/* read-eval-print-loop */
void	repl(void)
{
	t_list			*lines;
	t_list			*real_lines;
	unsigned char	exit_status;

	while (true)
	{
		lines = get_lines(STDIN_FILENO);
		real_lines = lnew();
		if (lines == NULL)
			break ;
		if (option_enabled('t'))
			lpush(real_lines, as_data(lines->first));
		else
			lextend(real_lines, lines);
		exit_status = interpret_lines(real_lines);
		if (option_enabled('t') || option_enabled('c')
			|| (exit_status != 0 && option_enabled('e')))
			break ;
		gc_free("DEFAULT");
	}
}

void	finish(bool print_exit)
{
	if (print_exit && !option_enabled('t') && option_enabled('i')
		&& !option_enabled('c') && !(get_last_exit_status() != 0
			&& option_enabled('e')))
		ft_dprintf(STDERR_FILENO, "exit\n");
	rl_clear_history();
	(void)gc_free_all();
}

void	handle_pwd_missing(void)
{
	char	*real_path;

	real_path = gc_add_str(getcwd(NULL, 0));
	if (real_path == NULL)
	{
		minishell_error(0, 0, "!shell-init: error retrieving "
			"current directory: getcwd: cannot access "
			"parent directory: %s", strerror(errno));
		set_saved_cwd("");
	}
	else
	{
		set_var("PWD", real_path, (t_flags){.exp = true});
		set_saved_cwd(real_path);
	}
}
