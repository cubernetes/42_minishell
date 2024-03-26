/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:01:02 by pgrussin          #+#    #+#             */
/*   Updated: 2024/03/25 12:42:58 by pgrussin         ###   ########.fr       */
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

t_bool	redirect_override(t_ast_node **override)
{
	int	fd;
	char	*str;
	t_bool	rtn;

	rtn = TRUE;
	//fd = open()
	return (rtn);
}

t_bool	execute_io_redirect(t_ast_node **io_redirect)
{
	t_fdrd *redir_fd;
	t_bool	rtn;
	t_ast_node	**children;

	children = (*io_redirect)->data.children;
	rtn = 0;
	redir_fd = (struct t_fdrd *)ft_malloc(sizeof(t_fdrd));
	if (!redir_fd)
		return (1);
	while (*children != NULL)
	{
		if ((*children)->data.token->type == TOK_OVERRIDE)
			rtn = redirect_override(children);
		children++;
	}
	return (rtn);
}

t_bool	execute_simple_command(t_ast_node **simple_command)
{
	t_ast_node	**children;
	t_bool		rtn;

	children = (*simple_command)->data.children;
	rtn = 0;
	while (*children != NULL)
	{
		if ((*children)->type == IO_REDIRECT)
				rtn = execute_io_redirect((*simple_command)->data.token);
		/*else if ((*children)->type == TOKEN)
			return (1);*/
		children++;
	}
	return (rtn);
}

t_bool	iterate_pipe_sequence(t_ast_node **commands)
{
	t_bool	rtn;
	t_ast_node	**children;

	rtn = 0;
	children = (*commands)->data.children;
	while (*children != NULL)
	{
		/* TODO: implement complete Command */
		if ((*children)->type == COMPLETE_COMMAND)
			rtn = 1;
		else if ((*children)->type == SIMPLE_COMMAND)
			rtn = execute_simple_command((*commands)->data.children);
		children++;
	}
	return (rtn);
}

t_bool	execute_pipe_sequence(t_ast_node **pipe_sequence)
{
	t_bool	rtn;
	/* TODO: set only onetime for unset PATH*/
	rtn = iterate_pipe_sequence(pipe_sequence);
	return (rtn);
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

t_bool	execute_token(t_ast_node **token)
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
t_bool	execute_complete_command(t_ast_node *ast_node)
{
	int	i;
	t_ast_node	**children;
	t_bool		b;
	
	children = ast_node->data.children;
	while (*children != NULL)
	{
		if (((*children)->type == PIPE_SEQUENCE) && (*(children + 1)) != NULL && 
		((*(children + 1))->data.token->type != TOK_AND && 
		(*(children + 1))->data.token->type != TOK_OR))
			b = execute_pipe_sequence((*children)->data.children);
		else if ((*children)->type == TOKEN)
		{
			if ((*children)->data.token->type == TOK_AND)
			{
				b = execute_tok_and(children);
			 	if (b == TRUE)
			       		b = execute_pipe_sequence((*children)->data.children);
			}
			else if ((*children)->data.token->type == TOK_OR)
			{
				b = execute_tok_or(children);
				if (b == FALSE)
			       		b = execute_pipe_sequence((*children)->data.children);
			}
		}
		children++;
	}
	return (b);
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	int rtn;
	if (ast_node == NULL)
		return ;
	rtn = execute_complete_command(ast_node);
	printf("return: %d\n", rtn);

}

