/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:01:02 by pgrussin          #+#    #+#             */
/*   Updated: 2024/02/22 18:24:50 by pgrussin         ###   ########.fr       */
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
	while (*simple_command != NULL)
	{
		if ((*simple_command)->type == IO_REDIRECT)
				execute_io_redirect((*simple_command)->data.token);
		simple_command++;
	}
	return (0);
}

int	iterate_pipe_sequence(t_ast_node **commands)
{
	int	rtn;
	while (*commands != NULL)
	{
		if ((*commands)->type == COMPLETE_COMMAND)
			rtn = 1;
		else if ((*commands)->type == SIMPLE_COMMAND)
			execute_simple_commands((*commands)->data.children);
		commands++;
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
		printf("TOK AND: \n");
	}
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
		if ((*children)->type == PIPE_SEQUENCE)
			rtn = execute_pipe_sequence((*children)->data.children);
		/*if (ast_node->data.children[i]->type == SIMPLE_COMMAND)
			rtn = execute_simple_command(*child);*/
		//should be TOK_AND, but somehow is only token
		/*if ((*children)->type == TOKEN)
		  if {
		  }
			rtn = execute_token(*children);
		if (ast_node->data.children[i]->type == TOK_OR)
			rtn = execute_tok_or(*child);*/
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
