/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:06:45 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 17:53:41 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <stdio.h>
#include <readline/readline.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/* TODO: What to do in case of execve error? */
#define EXECVE_ERR 3
#define FORK_ERROR 4

/* deliberately disable env, since wtf, env should NOT be a builtin. */
/* pedago fu'ed up here. what if you want to check if _ is working? CAN'T DO IT */
/* quoteth the bible (POSIX): "The env UTILITY shall obtain the current environment" */
		/* || ft_strcmp(word, "env") == 0 */
bool	is_builtin(char	*word)
{
	return (ft_strcmp(word, "cd") == 0
		|| ft_strcmp(word, "echo") == 0
		|| ft_strcmp(word, "exit") == 0
		|| ft_strcmp(word, "export") == 0
		|| ft_strcmp(word, "pwd") == 0
		|| ft_strcmp(word, "readonly") == 0
		|| ft_strcmp(word, "declare") == 0
		|| ft_strcmp(word, "source") == 0
		|| ft_strcmp(word, ".") == 0
		|| ft_strcmp(word, ":") == 0
		|| ft_strcmp(word, "shift") == 0
		|| ft_strcmp(word, "unset") == 0);
}

int	handle_builtin_helper(char **argv, t_fds fds)
{
	if (ft_strcmp(*argv, "source") == 0)
		return (builtin_source(argv, fds));
	else if (ft_strcmp(*argv, ".") == 0)
		return (builtin_source(argv, fds));
	else if (ft_strcmp(*argv, ":") == 0)
		return (builtin_colon(argv, fds));
	else if (ft_strcmp(*argv, "shift") == 0)
		return (builtin_shift(argv, fds));
	return (1);
}

int	handle_builtin(char	*argv[], t_fds fds)
{
	int	rtn;

	if (*argv == NULL)
		return (1);
	else if (ft_strcmp(*argv, "cd") == 0)
		return (builtin_cd(argv, fds));
	else if (ft_strcmp(*argv, "echo") == 0)
		return (builtin_echo(argv, fds));
	else if (ft_strcmp(*argv, "env") == 0)
		return (builtin_env(argv, fds));
	else if (ft_strcmp(*argv, "exit") == 0)
		return (builtin_exit(argv, fds));
	else if (ft_strcmp(*argv, "export") == 0)
		return (builtin_export(argv, fds));
	else if (ft_strcmp(*argv, "pwd") == 0)
		return (builtin_pwd(argv, fds));
	else if (ft_strcmp(*argv, "readonly") == 0)
		return (builtin_readonly(argv, fds));
	else if (ft_strcmp(*argv, "declare") == 0)
		return (builtin_declare(argv, fds));
	else if (ft_strcmp(*argv, "unset") == 0)
		return (builtin_unset(argv, fds));
	rtn = handle_builtin_helper(argv, fds);
	return (rtn);
}
