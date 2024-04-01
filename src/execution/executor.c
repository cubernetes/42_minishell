#include "../../include/minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* dup() */
#include <fcntl.h>

/* TODO: implement heredoc redirection*/
/*t_bool redirect_heredoc(t_ddeque *heredoc, t_ast_node *simple_command)
{
	t_bool		rtn;
	int		fd;
}*/

t_bool	redirect_input(char *file_path, t_ast_node *simple_command)
{
	t_bool	rtn;
	int		fd;
	int		sc_fd_in;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		simple_command->simple_cmd_meta.exit_status = 1;
		rtn = FALSE;
	}
	else
	{
		sc_fd_in = simple_command->simple_cmd_meta.fds.in;
		if (sc_fd_in != -2)
			close(sc_fd_in);
		simple_command->simple_cmd_meta.fds.out = fd;
		rtn = TRUE;
	}
	return (rtn);
}

t_bool	redirect_append(char *file_path, t_ast_node *simple_command)
{
	t_bool	rtn;
	int		fd;
	int		sc_fd_out;

	fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
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

t_bool	redirect_override(char *file_path, t_ast_node *simple_command)
{
	t_bool	rtn;
	int		fd;
	int		sc_fd_out;

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
	char			*file_path;
	t_token_type	type;

	rtn = FALSE;
	type = io_redirect->children->head->as_ast_node->token->type;
	file_path = io_redirect->children->head->next->as_ast_node->token->str;
	if (type == TOK_OVERRIDE)
		rtn = redirect_override(file_path, simple_command);
	else if (type == TOK_INPUT)
		rtn = redirect_input(file_path, simple_command);
	else if (type == TOK_APPEND)
		rtn = redirect_append(file_path, simple_command);
	/* else if (type == TOK_HEREDOC) */
		/* rtn = redirect_heredoc(file_path, simple_command); */
	if (rtn == FALSE)
		minishell_error(EXIT_FAILURE, "redirect error: %s", strerror(errno));
	return (rtn);
}

t_bool	execute_simple_command(t_ast_node *simple_command)
{
	t_bool			rtn;
	t_di			*di;

	rtn = TRUE;
	di = di_begin(simple_command->children);
	while (di_next(di))
		if (di_get(di)->as_ast_node->type == IO_REDIRECT)
			rtn = execute_io_redirect(di_get(di)->as_ast_node, simple_command);
	ms_execve(simple_command);
	return (rtn);
}

/* TODO: Not required: think about set -o pipefail */
t_bool	iterate_pipe_sequence(t_deque *commands)
{
	t_bool			rtn;
	t_di			*di;

	rtn = TRUE;
	di = di_begin(commands);
	while (di_next(di))
	{
		/* TODO: implement COMPLETE_COMMAND */
		if (di_get(di)->as_ast_node->type == COMPLETE_COMMAND)
			rtn = TRUE;
		else if (di_get(di)->as_ast_node->type == SIMPLE_COMMAND)
			rtn = execute_simple_command(di_get(di)->as_ast_node);
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

t_bool	execute_tok_and(t_deque_node *tok_and)
{
	t_ast_node	*left;

	/* next 2 lines can be removed */
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

	/* next 2 lines can be removed */
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
	t_bool			first;
	t_di			*di;

	first = TRUE;
	rtn = TRUE;
	di = di_begin(ast_node->children);
	while (di_next(di))
	{
		if (di_get(di)->as_ast_node->type == PIPE_SEQUENCE
			&& di_get(di)->next->as_ast_node->type != TOKEN)
			rtn = execute_pipe_sequence(di_get(di)->as_ast_node->children);
		else if (di_get(di)->as_ast_node->type == TOKEN)
		{
			if (di_get(di)->as_ast_node->token->type == TOK_AND)
			{
				if (first)
					rtn = execute_tok_and(di_get(di));
				first = FALSE;
				if (rtn == TRUE)
					rtn = execute_pipe_sequence(di_get(di)->next->as_ast_node->children);
				di_next(di);
			}
			else if (di_get(di)->as_ast_node->token->type == TOK_OR)
			{
				if (first)
					rtn = execute_tok_or(di_get(di));
				first = FALSE;
				if (rtn == FALSE)
					rtn = execute_pipe_sequence(di_get(di)->next->as_ast_node->children);
				di_next(di);
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
