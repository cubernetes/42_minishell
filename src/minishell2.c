/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:00:35 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 14:20:30 by pgrussin         ###   ########.fr       */
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

unsigned char	exec(t_tree *tree)
{
	unsigned char	exit_status;

	if (tree == NULL)
		exit_status = 0;
	else
		exit_status = execute_complete_command(tree);
	return (exit_status);
}

unsigned char	setup_initial_conditions(t_list *lines)
{
	set_var("LINENO", ft_itoa(ft_atoi(var_lookup("LINENO")) + 1),
		get_flags("LINENO"));
	if (get_var("MINISHELL_SOURCE_EXECUTION_STRING")
		&& get_var("MINISHELL_SOURCE_EXECUTION_STRING")->value)
	{
		lextend_left(lines,
			lsplit(var_lookup("MINISHELL_SOURCE_EXECUTION_STRING"), "\n"));
		set_var("MINISHELL_SOURCE_EXECUTION_STRING",
			NULL, get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
	}
	return (0); // Assuming the initial setup always succeeds
}

unsigned char	process_lines(t_list *lines)
{
	t_tree			*tree;
	unsigned char	exit_status;

	exit_status = get_last_exit_status();
	while (lines->len > 0)
	{
		set_var("LINENO", ft_itoa(ft_atoi(var_lookup("LINENO")) + 1),
			get_flags("LINENO"));
		set_var("CURRENT_LINE", lines->first->as_str, (t_flags){0});
		if (option_enabled('v'))
			ft_dprintf(STDERR_FILENO, "%s\n", lines->first->as_str);
		tree = parse(lines->first->as_str);
		if ((heredoc_aborted(-1) == false || tree == NULL)
			&& !option_enabled('n'))
			exit_status = exec(tree);
		lpop_left(lines);
		if (exit_status != 0 && option_enabled('e'))
			break ;
	}
	return (exit_status);
}

unsigned char	interpret_lines(t_list *lines)
{
	unsigned char	exit_status;

	exit_status = setup_initial_conditions(lines);
	if (exit_status == 0)
		exit_status = process_lines(lines);
	return (exit_status);
}

bool	option_enabled(char opt)
{
	return (ft_strchr(var_lookup("-"), opt) != NULL);
}
