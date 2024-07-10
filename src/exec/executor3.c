/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:03 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 19:29:24 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/* TODO: Test for unused headers */
#include <errno.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> /* dup() */
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

/* TODO: What to do (and return) in case of wait error? */
#define EXIT_WAIT_ERROR 4

/* TODO: Not required: think about set -o pipefail */
/* return last status */
/* TODO: Exit code 126 -> chmod -x */
/* TODO: Clean up this mess */
int	wait_pipe_sequence_helper(t_list *pids)
{
	int	status;

	if (pids->current->as_int == -1)
		status = 127;
	else if (pids->current->as_int <= -2 && pids->current->as_int >= -257)
		status = pids->current->as_int + 257;
	else if (pids->current->as_int == -258)
		status = 0;
	return (status);
}

unsigned char	wait_pipe_sequence(t_list *pids)
{
	int		rtn;
	int		status;

	liter(pids);
	while (lnext(pids))
	{
		if (pids->current->as_int < 0)
		{
			status = wait_pipe_sequence_helper(pids);
			continue ;
		}
		noninteractive_signals();
		rtn = waitpid(pids->current->as_pid_t, &status, 0);
		if (rtn == -1)
			minishell_error(EXIT_WAIT_ERROR, false, false, "wait error: %s",
				strerror(errno));
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = 128 + WTERMSIG(status);
		else
			minishell_error(EXIT_WAIT_ERROR, false, false,
				"process ended unexpectedly", strerror(errno));
	}
	return ((unsigned char)status);
}

void	setup_pipes(t_list *commands)
{
	t_tree		*prev;
	int			fds[2];

	liter(commands);
	prev = NULL;
	while (lnext(commands))
	{
		commands->current->as_tree->fd_in = -2;
		commands->current->as_tree->fd_out = -2;
		commands->current->as_tree->fd_err = -2;
		if (prev == NULL)
		{
			prev = commands->current->as_tree;
			continue ;
		}
		pipe(fds);
		if (prev->fd_out != -2)
			close(prev->fd_out);
		if (commands->current->as_tree->fd_in != -2)
			close(commands->current->as_tree->fd_in);
		prev->fd_out = fds[1];
		commands->current->as_tree->fd_in = fds[0];
		prev = commands->current->as_tree;
	}
}

unsigned char	execute_pipe_sequence(t_list *commands, bool negated)
{
	t_list			*pids;
	unsigned char	exit_status;

	setup_pipes(commands);
	pids = lnew();
	liter(commands);
	while (lnext(commands))
	{
		if (commands->current->as_tree->type == COMPLETE_COMMAND)
			lpush(pids, as_pid_t(execute_complete_command_wrapper(
						commands->current->as_tree, commands)));
		else if (commands->current->as_tree->type == SIMPLE_COMMAND)
			lpush(pids, as_pid_t(execute_simple_command_wrapper(
						commands->current->as_tree, commands)));
	}
	exit_status = wait_pipe_sequence(pids);
	if (negated)
		return (!(bool)exit_status);
	return (exit_status);
}

unsigned char	execute_tok_and(t_list_node *tok_and)
{
	t_tree	*left;

	left = tok_and->prev->as_tree;
	return (execute_pipe_sequence(left->children, left->negated));
}
