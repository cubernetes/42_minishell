#include "../../include/minishell.h"
#include <unistd.h> /* dup() */
#include <sys/types.h> /* open() */
#include <sys/stat.h>
#include <fcntl.h>

t_bool	redirect_override(t_deque *io_redirect_node, t_ast_node *simple_command)
{
	t_bool	rtn;
	int		fd;
	int		sc_fd_out;
	char	*file_path;

	file_path = io_redirect_node->head->next->as_ast_node->token->str;
	fd = open(file_path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
	{
		simple_command->simple_cmd_meta.exit_status = 1;
		rtn = FALSE;
	}
	else
	{
		sc_fd_out = simple_command->simple_cmd_meta.fds.out;
		if (sc_fd_out != -2)
			close(sc_fd_out);
		simple_command->simple_cmd_meta.fds.out = fd;
		rtn = TRUE;
	}
	return (rtn);
}

t_bool	execute_io_redirect(t_ast_node *io_redirect, t_ast_node *simple_command)
{
	t_bool			rtn;
	t_deque			*children;
	t_deque_node	*head;

	children = io_redirect->children;
	rtn = FALSE;
	head = children->head;
	if (!head)
		return (TRUE);
	if (head->as_ast_node->token->type == TOK_OVERRIDE)
		rtn = redirect_override(children, simple_command);
	while (head->next != children->head)
	{
		head = head->next;
		if (head->as_ast_node->token->type == TOK_OVERRIDE)
			rtn = redirect_override(children, simple_command);
	}
	return (rtn);
}

t_bool	execute_simple_command(t_ast_node *simple_command)
{
	t_deque			*children;
	t_deque_node	*head;
	t_bool			rtn;

	children = simple_command->children;
	rtn = FALSE;
	head = children->head;
	if (!head)
		return (TRUE);
	if (head->as_ast_node->type == IO_REDIRECT)
		rtn = execute_io_redirect(head->as_ast_node, simple_command);
	while (head->next != children->head)
	{
		head = head->next;
		if (head->as_ast_node->type == IO_REDIRECT)
			rtn = execute_io_redirect(head->as_ast_node, simple_command);
	}
	return (rtn);
}

t_bool	iterate_pipe_sequence(t_deque *commands)
{
	t_bool			rtn;
	t_deque_node	*head;

	rtn = FALSE;
	head = commands->head;
	if (!head)
		return (TRUE);
	if (head->as_ast_node->type == COMPLETE_COMMAND)
		rtn = TRUE;
	else if (head->as_ast_node->type == SIMPLE_COMMAND)
		rtn = execute_simple_command(head->as_ast_node);
	while (head->next != commands->head)
	{
		head = head->next;
		/* TODO: implement COMPLETE_COMMAND */
		if (head->as_ast_node->type == COMPLETE_COMMAND)
			rtn = TRUE;
		else if (head->as_ast_node->type == SIMPLE_COMMAND)
			rtn = execute_simple_command(head->as_ast_node);
	}
	return (rtn);
}

t_bool	execute_pipe_sequence(t_deque *pipe_sequence)
{
	t_bool	rtn;
	/* TODO: set only onetime for unset PATH*/
	rtn = iterate_pipe_sequence(pipe_sequence);
	return (rtn);
}

t_ast_node_type	give_ast_node_type(t_ast_node *ast_node)
{
	t_deque			*children;
	t_deque_node	*head;

	children = ast_node->children;
	head = children->head;
	if (!head)
		return (AST_NODE_TYPE_UNKNOWN);
	if (head->as_ast_node->type == COMPLETE_COMMAND)
		return (COMPLETE_COMMAND);
	else if (head->as_ast_node->type == TOKEN)
		return (TOKEN);
	else if (head->as_ast_node->type == PIPE_SEQUENCE)
		return (PIPE_SEQUENCE);
	while (head->next != children->head)
	{
		head = head->next;
		if (head->as_ast_node->type == COMPLETE_COMMAND)
			return (COMPLETE_COMMAND);
		else if (head->as_ast_node->type == TOKEN)
			return (TOKEN);
		else if (head->as_ast_node->type == PIPE_SEQUENCE)
			return (PIPE_SEQUENCE);
	}
	return (AST_NODE_TYPE_UNKNOWN);
}

t_bool	execute_token(t_ast_node *token)
{
	t_ast_node	*child;

	child = token->children->head->as_ast_node;
	if (give_ast_node_type(child) != AST_NODE_TYPE_UNKNOWN)
	{
		if (give_ast_node_type(child) == TOK_AND)
			return (TRUE);
	}
	return (FALSE);
}

t_bool	execute_tok_and(t_deque_node *tok_and)
{
	t_ast_node	*left;

	if (tok_and->prev->as_ast_node->type != PIPE_SEQUENCE)
		return (FALSE);
	left = tok_and->prev->as_ast_node;
	if (execute_pipe_sequence(left->children))
		return (TRUE);
	return (FALSE);
}

t_bool	execute_tok_or(t_deque_node *tok_or)
{
	t_ast_node	*left;

	/* TODO: protect the -1 (update from timo, this todo might be out-of-date */
	if (tok_or->prev->as_ast_node->type != PIPE_SEQUENCE)
		return (FALSE);
	left = tok_or->prev->as_ast_node;
	if (!execute_pipe_sequence(left->children))
		return (TRUE);
	return (FALSE);
}

/* return value for TOK_AND & TOK_OR to see if command was succesfully executed */
t_bool	execute_complete_command(t_ast_node *ast_node)
{
	t_bool			rtn;
	t_deque			*children;
	t_deque_node	*head;
	t_bool			first;

	children = ast_node->children;
	head = children->head;
	if (!head)
		return (TRUE);
	rtn = TRUE;
	first = TRUE;
	while (head->next != children->head)
	{
		head = head->next;
		if (head->as_ast_node->type == PIPE_SEQUENCE
			&& head->next->as_ast_node->type != TOKEN)
			rtn = execute_pipe_sequence(head->as_ast_node->children);
		else if (head->as_ast_node->type == TOKEN)
		{
			if (head->as_ast_node->token->type == TOK_AND)
			{
				if (first)
					rtn = execute_tok_and(head);
				first = FALSE;
				if (rtn == TRUE)
					rtn = execute_pipe_sequence(head->next->as_ast_node->children);
				head = head->next;
			}
			else if (head->as_ast_node->token->type == TOK_OR)
			{
				if (first)
					rtn = execute_tok_or(head);
				first = FALSE;
				if (rtn == FALSE)
					rtn = execute_pipe_sequence(head->next->as_ast_node->children);
				head = head->next;
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
