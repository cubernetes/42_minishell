#include "../../include/minishell.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> /* dup() */
#include <fcntl.h>
#include <sys/wait.h>

/* TODO: implement heredoc redirection*/
/* t_bool redirect_heredoc(t_ddeque *heredoc, t_ast_node *simple_command)
{
	return (TRUE);
} */

t_bool	handle_redirect_input(char *file_path, t_ast_node *simple_command)
{
	int	fd;
	int	sc_fd_in;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (simple_command->simple_cmd_meta.exit_status = 1, FALSE);
	else
	{
		sc_fd_in = simple_command->simple_cmd_meta.fds.in;
		if (sc_fd_in != -2)
			close(sc_fd_in);
		simple_command->simple_cmd_meta.fds.in = fd;
		return (TRUE);
	}
}

t_bool	handle_redirect_append(char *file_path, t_ast_node *simple_command)
{
	int		fd;
	int		sc_fd_out;

	fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		return (simple_command->simple_cmd_meta.exit_status = 1, FALSE);
	else
	{
		sc_fd_out = simple_command->simple_cmd_meta.fds.out;
		if (sc_fd_out != -2)
			close(sc_fd_out);
		simple_command->simple_cmd_meta.fds.out = fd;
		return (TRUE);
	}
}

t_bool	handle_redirect_override(char *file_path, t_ast_node *simple_command)
{
	int		fd;
	int		sc_fd_out;

	fd = open(file_path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		return (simple_command->simple_cmd_meta.exit_status = 1, FALSE);
	else
	{
		sc_fd_out = simple_command->simple_cmd_meta.fds.out;
		if (sc_fd_out != -2)
			close(sc_fd_out);
		simple_command->simple_cmd_meta.fds.out = fd;
		return (TRUE);
	}
}

void	handle_io_redirect(t_ast_node *io_redirect, t_ast_node *simple_command)
{
	t_bool			err;
	char			*file_path;
	t_token_type	type;

	type = io_redirect->children->head->as_ast_node->token->type;
	file_path = io_redirect->children->head->next->as_ast_node->token->str;
	if (type == TOK_OVERRIDE)
		err = handle_redirect_override(file_path, simple_command);
	else if (type == TOK_INPUT)
		err = handle_redirect_input(file_path, simple_command);
	else if (type == TOK_APPEND)
		err = handle_redirect_append(file_path, simple_command);
	/* else if (type == TOK_HEREDOC) */
		/* err = redirect_heredoc(file_path, simple_command); */
	if (err == FALSE)
		minishell_error(EXIT_FAILURE, TRUE,
			"redirect error: %s", strerror(errno));
}

pid_t	execute_simple_command_wrapper(t_ast_node *simple_command,
	t_deque *commands)
{
	t_di	*di;

	di = di_begin(simple_command->children);
	while (di_next(di))
		if (di_get(di)->as_ast_node->type == IO_REDIRECT)
			handle_io_redirect(di_get(di)->as_ast_node, simple_command);
	return (execute_simple_command(simple_command, commands));
}

void	print_pid(void *ptr, t_bool first)
{
	if (first)
		ft_printf("%d", *(pid_t *)ptr);
	else
		ft_printf(" -> %d", *(pid_t *)ptr);
}

/* TODO: What to do (and return) in case of wait error? */
#define EXIT_WAIT_ERROR 4

/* TODO: Not required: think about set -o pipefail */
/* return last status */
unsigned char	wait_pipesequence(t_deque *pids)
{
	t_di	*di;
	int		ret;
	int		status;

	di = di_begin(pids);
	while (di_next(di))
	{
		ret = waitpid(*(pid_t *)di_get(di)->as_ptr, &status, 0);
		if (ret == -1)
			minishell_error(EXIT_WAIT_ERROR, FALSE, "wait error: %d",
				strerror(errno));
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = 128 + WTERMSIG(status);
		ft_printf("EXIT STATUS: %d\n", status);
	}
	return ((unsigned char)status);
}

void	setup_pipes(t_deque *commands)
{
	t_di		*di;
	t_ast_node	*prev;
	int			fds[2];

	di = di_begin(commands);
	prev = NULL;
	while (di_next(di))
	{
		ms_init(di_get(di)->as_ast_node);
		if (prev == NULL)
		{
			prev = di_get(di)->as_ast_node;
			continue ;
		}
		pipe(fds);
		if (prev->simple_cmd_meta.fds.out != -2)
			close(prev->simple_cmd_meta.fds.out);
		if (di_get(di)->as_ast_node->simple_cmd_meta.fds.in != -2)
			close(di_get(di)->as_ast_node->simple_cmd_meta.fds.in);
		prev->simple_cmd_meta.fds.out = fds[1];
		di_get(di)->as_ast_node->simple_cmd_meta.fds.in = fds[0];
		prev = di_get(di)->as_ast_node;
	}
}

/* TODO: implement COMPLETE_COMMAND execution */
unsigned char	iterate_pipe_sequence(t_deque *commands)
{
	t_di			*di;
	unsigned char	status;
	t_deque			*pids;

	setup_pipes(commands);
	pids = deque_init();
	di = di_begin(commands);
	while (di_next(di))
	{
		if (di_get(di)->as_ast_node->type == COMPLETE_COMMAND)
			;
			/* TODO: Implement ./minishell -c '' functionality */
			/*       or add new metadata for COMPLETE_COMMAND (fds) */
		else if (di_get(di)->as_ast_node->type == SIMPLE_COMMAND)
		{
			ast_print(di_get(di)->as_ast_node);
			deque_push_ptr_right(pids, ft_memdup(&(pid_t){\
				execute_simple_command_wrapper(di_get(di)->as_ast_node, commands)},
					sizeof(pid_t)));
		}
	}
	status = wait_pipesequence(pids);
	ft_printf("\033[31mPipe sequence exited with: %d\033[m\n", status);
	return (status);
}

unsigned char	execute_pipe_sequence(t_deque *pipe_sequence)
{
	t_bool	rtn;
	/* TODO: set only onetime for unset PATH */
	rtn = iterate_pipe_sequence(pipe_sequence);
	return (rtn);
}

unsigned char	execute_tok_and(t_deque_node *tok_and)
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

unsigned char	execute_tok_or(t_deque_node *tok_or)
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
unsigned char	execute_complete_command(t_ast_node *ast_node)
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
/* TODO: think if 0 (success) is a sensible return for ast_node == NULL */
unsigned char	execute(t_ast_node *ast_node)
{
	t_bool	rtn;

	if (ast_node == NULL)
		return (0);
	rtn = execute_complete_command(ast_node);
	return (rtn);
}
