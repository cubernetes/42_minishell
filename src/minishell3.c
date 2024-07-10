/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:20:50 by pgrussin          #+#    #+#             */
/*   Updated: 2024/07/10 22:17:30 by tischmid         ###   ########.fr       */
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

static void	helper_func(int *helper)
{
	dup2(*helper, STDERR_FILENO);
	close(*helper);
}

static void	get_lines_helper(char **input, char **ps1,
int *helper)
{
	bool	restore;

	restore = false;
	if (!isatty(STDERR_FILENO))
	{
		restore = true;
		*helper = open("/dev/tty", O_WRONLY);
		if (*helper == -1)
			minishell_error(1, 0, "/dev/tty: ", strerror(errno));
		else
		{
			dup2(*helper, STDERR_FILENO);
			close(*helper);
			*helper = dup(STDERR_FILENO);
		}
		if (!option_enabled('i'))
			*ps1 = "";
	}
	interactive_signals();
	*input = readline(*ps1);
	noninteractive_signals();
	if (restore)
		helper_func(&helper);
	gc_add(*input);
}

static void	get_lines_helper2(int *fd, char *ps1, char **input)
{
	*input = get_next_line(*fd);
	if (*input != NULL && option_enabled('i'))
	{
		ft_dprintf(STDERR_FILENO, "%s", ft_strjoin(ps1, *input));
		if ((*input)[strlen(*input) - 1] != '\n')
			ft_dprintf(STDERR_FILENO, "%s", "\n");
	}
	else if (option_enabled('i'))
		ft_dprintf(STDERR_FILENO, "%s", ps1);
	if (*input != NULL && (*input)[strlen(*input) - 1] == '\n')
		(*input)[strlen(*input) - 1] = '\0';
}

static t_list	*get_lines_helper3(void)
{
	char	*input;

	input = var_lookup("MINISHELL_SOURCE_EXECUTION_STRING");
	set_var("MINISHELL_SOURCE_EXECUTION_STRING", NULL,
		get_flags("MINISHELL_SOURCE_EXECUTION_STRING"));
	return (lsplit(input, "\n"));
}

t_list	*get_lines(int fd)
{
	char		*input;
	char		*ps1;
	static char	*prev_input;
	int			helper;

	prev_input = "";
	if (option_enabled('c'))
		return (lsplit(var_lookup("MINISHELL_EXECUTION_STRING"), "\n"));
	else if (get_var("MINISHELL_SOURCE_EXECUTION_STRING")
		&& get_var("MINISHELL_SOURCE_EXECUTION_STRING")->value)
		return (get_lines_helper3);
	ps1 = expand_prompt(var_lookup("PS1"));
	if (isatty(STDIN_FILENO))
		get_lines_helper(&input, &ps1, &helper);
	else
		get_lines_helper2(&fd, &ps1, &input);
	if (input == NULL)
		return (NULL);
	if (*input && ft_strcmp(input, prev_input))
		add_history(input);
	ft_dprintf(STDERR_FILENO, "%s", var_lookup("PS0"));
	gc_start_context("POST");
	prev_input = ft_strdup(input);
	gc_end_context();
	return (lsplit(input, "\n"));
}
