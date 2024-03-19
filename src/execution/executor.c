/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:01:02 by pgrussin          #+#    #+#             */
/*   Updated: 2024/03/19 17:11:34 by pgrussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/*dup lib*/
#include <unistd.h>
/*open lib*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*malloc lib*/
#include <stdlib.h>
/* TODO: delete, used for printf*/
#include <stdio.h>

int	fd_init(t_fdrd *redir_fd, t_ast_node **redirections)
{
	if ((*redirections)->type == TOK_HEREDOC)
		return (1);
}

int	execute_io_redirect(t_ast_node **io_redirect)
{
	t_fdrd *redir_fd;
	int	rtn;

	rtn = 1;
	redir_fd = (struct t_fdrd *)ft_malloc(sizeof(t_fdrd));
	if (!redir_fd)
		return (1);
	while (*io_redirect != NULL)
	{
		if ((*io_redirect)->data.token->type == TOK_HEREDOC)
			fd_init(redir_fd, (*io_redirect));
		io_redirect++;
	}
}

int	execute_simple_commands(t_ast_node **simple_command)
{
	t_ast_node	**children;

	children = (*simple_command)->data.children;
	while (*children != NULL)
	{
		if ((*children)->type == IO_REDIRECT)
				execute_io_redirect((*simple_command)->data.token);
		else if ((*children)->type == TOKEN)
			return (1);
		children++;
	}
	return (0);
}

int	iterate_pipe_sequence(t_ast_node **commands)
{
	int	rtn;
	t_ast_node	**children;

	children = (*commands)->data.children;
	while (*children != NULL)
	{
		/* TODO: implement complete Command */
		if ((*children)->type == COMPLETE_COMMAND)
			rtn = 1;
		else if ((*children)->type == SIMPLE_COMMAND)
			execute_simple_commands((*commands)->data.children);
		children++;
	}
	return (0);
}

int	execute_pipe_sequence(t_ast_node **pipe_sequence)
{
	/* TODO: set only onetime for unset PATH*/

	char **envp_tmp = get_environ();
	iterate_pipe_sequence(pipe_sequence);
	return (0);
}

t_ast_node_type	give_ast_node_type(t_ast_node **ast_node)
{
	t_ast_node	**children;
	children = (*ast_node)->data.children;
	if (*children != NULL)
	{
		if ((*children)->type == COMPLETE_COMMAND)
			return (COMPLETE_COMMAND);
		if ((*children)->type == TOKEN)
			return (TOKEN);
		else if ((*children)->type == PIPE_SEQUENCE)
			return (PIPE_SEQUENCE);
		children++;
	}
	return (NULL);
}

int	execute_token(t_ast_node **token)
{
	t_ast_node	**child;

	child = (*token)->data.children;
	if (give_ast_node_type(child) != NULL)
	{
		if (give_ast_node_type == TOK_AND)
			return (1);

	}
}

t_bool	execute_tok_and(t_ast_node **tok_and)
{
	t_ast_node	**parent;

	if (*(tok_and - 1) == NULL)
		return (FALSE);
	parent = tok_and - 1;
	if (execute_pipe_sequence(parent))
	{
		return (TRUE);
	}
	return (FALSE);
}

t_bool	execute_tok_or(t_ast_node **tok_or)
{
	t_ast_node	**parent;
	/* TODO: protect the -1*/
	if (*(tok_or - 1) == NULL)
		return (FALSE);
	parent = tok_or - 1;
	if (!execute_pipe_sequence(parent))
	{
		return (TRUE);
	}
	return (FALSE);
}
/*return value for TOK_AND & TOK_OR to see if command was succesful executet*/
int	execute_complete_command(t_ast_node *ast_node)
{
	int	i;
	int	rtn;
	t_ast_node **children;
	
	rtn = 1;
	children = ast_node->data.children;
	while (*children != NULL)
	{
		if (((*children)->type == PIPE_SEQUENCE) && (*(children + 1)) != NULL && 
		((*(children + 1))->data.token->type != TOK_AND && 
		(*(children + 1))->data.token->type != TOK_OR))
			rtn = execute_pipe_sequence((*children)->data.children);
		else if ((*children)->type == TOKEN)
		{
			if ((*children)->data.token->type == TOK_AND)
			       execute_tok_and(children);
			else if ((*children)->data.token->type == TOK_OR)
				execute_tok_or(children);
		}
		children++;
	}
	return (rtn);
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	if (ast_node == NULL)
		return ;
	execute_complete_command(ast_node);
}
