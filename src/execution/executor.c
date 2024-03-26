/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:01:02 by pgrussin          #+#    #+#             */
/*   Updated: 2024/03/26 10:07:46 by tosuman          ###   ########.fr       */
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

t_bool	redirect_override(t_ddeque *override)
{
	t_bool	rtn;

	/* int	fd; */
	/* char	*str; */
	//fd = open()
	(void)override;
	rtn = TRUE;
	return (rtn);
}

t_bool	execute_io_redirect(t_ast_node *io_redirect)
{
	t_fdrd			*redir_fd;
	t_bool			rtn;
	t_ddeque		*children;
	t_ddeque_node	*head;

	children = io_redirect->data.children;
	rtn = FALSE;
	redir_fd = ft_malloc(sizeof(*redir_fd));
	head = children->head;
	if (!head)
		return (TRUE);
	if (((t_ast_node *)head->data)->data.token->type == TOK_OVERRIDE)
		rtn = redirect_override(children);
	while (head->next != children->head)
	{
		head = head->next;
		if (((t_ast_node *)head->data)->data.token->type == TOK_OVERRIDE)
			rtn = redirect_override(children);
	}
	return (rtn);
}

t_bool	execute_simple_command(t_ast_node *simple_command)
{
	t_ddeque		*children;
	t_ddeque_node	*head;
	t_bool			rtn;

	children = simple_command->data.children;
	rtn = FALSE;
	head = children->head;
	if (!head)
		return (TRUE);
	if (((t_ast_node *)head->data)->type == IO_REDIRECT)
		rtn = execute_io_redirect(head->data);
	while (head->next != children->head)
	{
		head = head->next;
		if (((t_ast_node *)head->data)->type == IO_REDIRECT)
			rtn = execute_io_redirect(head->data);
	}
	return (rtn);
}

t_bool	iterate_pipe_sequence(t_ddeque *commands)
{
	t_bool			rtn;
	t_ddeque_node	*head;

	rtn = FALSE;
	head = commands->head;
	if (!head)
		return (TRUE);
	if (((t_ast_node *)head->data)->type == COMPLETE_COMMAND)
		rtn = TRUE;
	else if (((t_ast_node *)head->data)->type == SIMPLE_COMMAND)
		rtn = execute_simple_command(head->data);
	while (head->next != commands->head)
	{
		head = head->next;
		/* TODO: implement COMPLETE_COMMAND */
		if (((t_ast_node *)head->data)->type == COMPLETE_COMMAND)
			rtn = TRUE;
		else if (((t_ast_node *)head->data)->type == SIMPLE_COMMAND)
			rtn = execute_simple_command(head->data);
	}
	return (rtn);
}

t_bool	execute_pipe_sequence(t_ddeque *pipe_sequence)
{
	t_bool	rtn;
	/* TODO: set only onetime for unset PATH*/
	rtn = iterate_pipe_sequence(pipe_sequence);
	return (rtn);
}

t_ast_node_type	give_ast_node_type(t_ast_node *ast_node)
{
	t_ddeque		*children;
	t_ddeque_node	*head;

	children = ast_node->data.children;
	head = children->head;
	if (!head)
		return (AST_NODE_TYPE_UNKNOWN);
	if (((t_ast_node *)head->data)->type == COMPLETE_COMMAND)
		return (COMPLETE_COMMAND);
	else if (((t_ast_node *)head->data)->type == TOKEN)
		return (TOKEN);
	else if (((t_ast_node *)head->data)->type == PIPE_SEQUENCE)
		return (PIPE_SEQUENCE);
	while (head->next != children->head)
	{
		head = head->next;
		if (((t_ast_node *)head->data)->type == COMPLETE_COMMAND)
			return (COMPLETE_COMMAND);
		else if (((t_ast_node *)head->data)->type == TOKEN)
			return (TOKEN);
		else if (((t_ast_node *)head->data)->type == PIPE_SEQUENCE)
			return (PIPE_SEQUENCE);
	}
	return (AST_NODE_TYPE_UNKNOWN);
}

t_bool	execute_token(t_ast_node *token)
{
	t_ast_node	*child;

	child = token->data.children->head->data;
	if (give_ast_node_type(child) != AST_NODE_TYPE_UNKNOWN)
	{
		if (give_ast_node_type(child) == TOK_AND)
			return (TRUE);
	}
	return (FALSE);
}

t_bool	execute_tok_and(t_ddeque_node *tok_and)
{
	t_ast_node	*left;

	if (((t_ast_node *)tok_and->prev->data)->type != PIPE_SEQUENCE)
		return (FALSE);
	left = ((t_ast_node *)tok_and->prev->data);
	if (execute_pipe_sequence(left->data.children))
		return (TRUE);
	return (FALSE);
}

t_bool	execute_tok_or(t_ddeque_node *tok_or)
{
	t_ast_node	*left;

	/* TODO: protect the -1 (update from timo, this todo might be out-of-date */
	if (((t_ast_node *)tok_or->prev->data)->type != PIPE_SEQUENCE)
		return (FALSE);
	left = ((t_ast_node *)tok_or->prev->data);
	if (!execute_pipe_sequence(left->data.children))
		return (TRUE);
	return (FALSE);
}

/* return value for TOK_AND & TOK_OR to see if command was succesfully executed */
t_bool	execute_complete_command(t_ast_node *ast_node)
{
	t_bool			rtn;
	t_ddeque		*children;
	t_ddeque_node	*head;

	children = ast_node->data.children;
	head = children->head;
	if (!head)
		return (TRUE);
	while (head->next != children->head)
	{
		head = head->next;
		if (((t_ast_node *)head->data)->type == PIPE_SEQUENCE
			&& ((t_ast_node *)head->next->data)->type != TOKEN)
			rtn = execute_pipe_sequence(((t_ast_node *)head->data)->data.children);
		else if (((t_ast_node *)head->data)->type == TOKEN)
		{
			if (((t_ast_node *)head->data)->data.token->type == TOK_AND)
			{
				rtn = execute_tok_and(head);
				if (rtn == TRUE)
					rtn = execute_pipe_sequence(((t_ast_node *)head->prev->data)->data.children);
			}
			else if (((t_ast_node *)head->data)->data.token->type == TOK_OR)
			{
				rtn = execute_tok_or(head);
				if (rtn == FALSE)
					rtn = execute_pipe_sequence(((t_ast_node *)head->prev->data)->data.children);
			}
			else
			{
				/* syntax error */
			}
		}
	}
	return (rtn);
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	t_bool	rtn;

	if (ast_node == NULL)
		return ;
	rtn = execute_complete_command(ast_node);
	ft_printf("return: %d\n", rtn);
}

