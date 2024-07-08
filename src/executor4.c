/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:10 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:31:11 by paul             ###   ########.fr       */
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

unsigned char	execute_tok_or(t_list_node *tok_or)
{
	t_tree	*left;

	left = tok_or->prev->as_tree;
	return (execute_pipe_sequence(left->children, left->negated));
}

unsigned char	execute_pipe_sequence_node(t_tree *pipe_sequence_node)
{
	return (execute_pipe_sequence(pipe_sequence_node->children,
			pipe_sequence_node->negated));
}

unsigned char	execute_token_and(t_list_node *token_node)
{
	unsigned char	rtn;

	rtn = execute_tok_and(token_node);
	set_last_exit_status(rtn);
	if (rtn == 0)
		rtn = execute_pipe_sequence_node(token_node->next->as_tree);
	return (rtn);
}

unsigned char	execute_token_or(t_list_node *token_node)
{
	unsigned char	rtn;

	rtn = execute_tok_or(token_node);
	set_last_exit_status(rtn);
	if (rtn != 0)
		rtn = execute_pipe_sequence_node(token_node->next->as_tree);
	return (rtn);
}

unsigned char	execute_token_semi(t_list_node *token_node)
{
	unsigned char	rtn;

	rtn = execute_tok_or(token_node);
	set_last_exit_status(rtn);
	if (rtn != 0 && option_enabled('e'))
		return (rtn);
	return (execute_pipe_sequence_node(token_node->next->as_tree));
}
