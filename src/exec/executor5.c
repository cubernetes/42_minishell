/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:14 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 23:34:38 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <errno.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

void	and_or_token(t_list_node *child_current, t_token *token,
	bool *first, unsigned char *rtn)
{
	if (token->type == TOK_AND)
		execute_token_and(child_current, first, rtn);
	else if (token->type == TOK_OR)
		execute_token_or(child_current, first, rtn);
	else if (token->type == TOK_SEMI)
		execute_token_semi(child_current, first, rtn);
}

unsigned char	execute_commands_in_node(t_list *children)
{
	unsigned char	rtn;
	t_list			*chldn;
	t_tree			*current_tree;
	bool			first;

	first = true;
	chldn = liter(children);
	rtn = get_last_exit_status();
	while (lnext(chldn))
	{
		current_tree = chldn->current->as_tree;
		if (current_tree->type == PIPE_SEQUENCE
			&& chldn->current->next->as_tree->type != TOKEN)
			rtn = execute_pipe_sequence_node(current_tree);
		else if (current_tree->type == TOKEN)
		{
			and_or_token(chldn->current, current_tree->token, &first,
				&rtn);
			if (lnext(chldn) == NULL)
				break ;
		}
	}
	set_last_exit_status(rtn);
	return (rtn);
}

unsigned char	execute_complete_command(t_tree *node)
{
	return (execute_commands_in_node(node->children));
}

#define EXIT_FORK_ERROR 4

pid_t	execute_complete_command_wrapper(t_tree *complete_command,
t_list *commands)
{
	pid_t	pid;
	int		rtn;

	pid = fork();
	if (pid < 0)
		minishell_error(EXIT_FORK_ERROR, 1, "%s", strerror(errno));
	else if (pid > 0)
		return (close_fds(complete_command), pid);
	set_fds(complete_command);
	close_other_command_fds(commands);
	rtn = execute_complete_command(complete_command);
	finish(false);
	exit(rtn);
}

unsigned char	execute(t_tree *node)
{
	unsigned char	rtn;

	if (node == NULL)
		return (0);
	rtn = execute_complete_command(node);
	return (rtn);
}
