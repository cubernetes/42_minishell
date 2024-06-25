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

void	redirect_heredoc(char *file_path, t_tree *simple_command)
{
	int			fd;
	int			new_hd_fd;
	int			sc_fd_in;
	t_fatptr	line;
	char		*new_hd;

	fd = open(file_path, O_RDONLY);
	new_hd = ft_mktemp("minishell.");
	new_hd_fd = open(new_hd, O_CREAT | O_TRUNC | O_RDWR, 0600);
	if (fd == -1 || new_hd_fd == -1)
	{
		if (fd != -1)
			close(fd);
		if (new_hd_fd != -1)
			close(new_hd_fd);
		simple_command->error = minishell_error(1, false,
				"%s: %s", file_path, strerror(errno));
	}
	else
	{
		while (1)
		{
			line = get_next_fat_line(fd);
			if (!line.data)
				break ;
			/* ft_dprintf(new_hd_fd, "%s", expand_parameters(strip_nul(line.data, line.size), "", true)->first->as_token->str); */
			ft_dprintf(new_hd_fd, "%s", strip_nul(line.data, line.size));
		}
		close(new_hd_fd);
		close(fd);
		new_hd_fd = open(new_hd, O_RDONLY);
		if (new_hd_fd == -1)
		{
			simple_command->error = minishell_error(1, false,
					"%s: %s", file_path, strerror(errno));
			return ;
		}
		sc_fd_in = simple_command->fd_in;
		if (sc_fd_in != -2)
			close(sc_fd_in);
		simple_command->fd_in = new_hd_fd;
		simple_command->error = 0;
	}
}

void	handle_redirect_input(char *file_path, t_tree *simple_command)
{
	int	fd;
	int	sc_fd_in;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		simple_command->error = minishell_error(1, false,
				"%s: %s", file_path, strerror(errno));
	else
	{
		sc_fd_in = simple_command->fd_in;
		if (sc_fd_in != -2)
			close(sc_fd_in);
		simple_command->fd_in = fd;
		simple_command->error = 0;
	}
}

void	handle_redirect_append(char *file_path, t_tree *simple_command)
{
	int		fd;
	int		sc_fd_out;

	fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		simple_command->error = minishell_error(1, false,
				"%s: %s", file_path, strerror(errno));
	else
	{
		sc_fd_out = simple_command->fd_out;
		if (sc_fd_out != -2)
			close(sc_fd_out);
		simple_command->fd_out = fd;
		simple_command->error = 0;
	}
}

void	handle_redirect_override(char *file_path, t_tree *simple_command)
{
	int		fd;
	int		sc_fd_out;

	fd = open(file_path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		simple_command->error = minishell_error(1, false,
				"%s: %s", file_path, strerror(errno));
	else
	{
		sc_fd_out = simple_command->fd_out;
		if (sc_fd_out != -2)
			close(sc_fd_out);
		simple_command->fd_out = fd;
		simple_command->error = 0;
	}
}

int	handle_io_redirect(t_tree *io_redirect, t_tree *simple_command)
{
	char			*file_path;
	t_token_type	type;

	type = io_redirect->children->first->as_tree->token->type;
	file_path = io_redirect->children->first->next->as_tree->token->str;
	if (io_redirect->children->first->next->as_tree->token->num_tokens_after_split != 1)
		return (simple_command->error = minishell_error(1, false, "%s: ambiguous redirect", "whatever"));
	if (type == TOK_OVERRIDE)
		handle_redirect_override(file_path, simple_command);
	else if (type == TOK_INPUT)
		handle_redirect_input(file_path, simple_command);
	else if (type == TOK_APPEND)
		handle_redirect_append(file_path, simple_command);
	else if (type == TOK_HEREDOC)
		redirect_heredoc(file_path, simple_command);
	else
	{
	}
	return (simple_command->error);
}

/* TODO: get rid of this wrapper mess */
/* TOOD: abstract away into new function */
pid_t	execute_simple_command_wrapper(t_tree *simple_command,
	t_list *commands)
{
	t_list	*new_children; // TODO: Shadows function new_children

	new_children = lnew();
	liter(simple_command->children);
	while (lnext(simple_command->children))
	{
		if (simple_command->children->current->as_tree->type == IO_REDIRECT
			&& liter(simple_command->children->current->as_tree->children))
			while (lnext(simple_command->children->current->as_tree->children))
				lpush(new_children, as_token(simple_command->children->current->as_tree->children->current->as_tree->token));
		else
			lpush(new_children, as_token(simple_command->children->current->as_tree->token));
	}
	lpush(new_children, as_token(new_token("", TOK_EOL, true)));
	new_children = expand_tokens(new_children);
	join_tokens(new_children);
	glob_tokens(new_children);
	if (new_children->len <= 1)
		return (close_fds(simple_command), -258);
	new_children = build_ast(new_children, false)->children->first->as_tree->children->first->as_tree->children;
	simple_command->children = new_children;
	liter(simple_command->children);
	while (lnext(simple_command->children))
		if (simple_command->children->current->as_tree->type == IO_REDIRECT)
			if (handle_io_redirect(simple_command->children->current->as_tree, simple_command))
				return (close_fds(simple_command), simple_command->error - 257);
	return (execute_simple_command(simple_command, commands));
}

/* TODO: What to do (and return) in case of wait error? */
#define EXIT_WAIT_ERROR 4

/* TODO: Not required: think about set -o pipefail */
/* return last status */
/* TODO: Exit code 126 -> chmod -x */
/* TODO: Clean up this mess */
unsigned char	wait_pipe_sequence(t_list *pids)
{
	int		rtn;
	int		status;

	liter(pids);
	while (lnext(pids))
	{
		if (pids->current->as_int < 0)
		{
			if (pids->current->as_int == -1)
				status = 127;
			else if (pids->current->as_int <= -2 && pids->current->as_int >= -257)
				status = pids->current->as_int + 257;
			else if (pids->current->as_int == -258)
				status = 0;
			continue ;
		}
		noninteractive_signals();
		rtn = waitpid(pids->current->as_pid_t, &status, 0);
		if (rtn == -1)
			minishell_error(EXIT_WAIT_ERROR, false, "wait error: %s",
				strerror(errno));
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = 128 + WTERMSIG(status);
		else
			minishell_error(EXIT_WAIT_ERROR, false, "process ended unexpectedly",
				strerror(errno));
	}
	return ((unsigned char)status);
}

void	setup_pipes(t_list *commands)
{
	t_tree	*prev;
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

pid_t	execute_complete_command_wrapper(t_tree *complete_command,
	t_list *commands);

unsigned char	execute_pipe_sequence(t_list *commands)
{
	t_list			*pids;

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
	return (wait_pipe_sequence(pids));
}

unsigned char	execute_tok_and(t_list_node *tok_and)
{
	t_tree	*left;

	left = tok_and->prev->as_tree;
	return (execute_pipe_sequence(left->children));
}

unsigned char	execute_tok_or(t_list_node *tok_or)
{
	t_tree	*left;

	left = tok_or->prev->as_tree;
	return (execute_pipe_sequence(left->children));
}

/* return value for TOK_AND & TOK_OR to see if command was succesfully executed */
unsigned char	execute_complete_command(t_tree *node)
{
	unsigned char	rtn;
	bool			first;
	t_list			*chldn;

	first = true;
	rtn = get_last_exit_status();
	chldn = liter(node->children);
	while (lnext(chldn))
	{
		if (chldn->current->as_tree->type == PIPE_SEQUENCE
			&& chldn->current->next->as_tree->type != TOKEN)
			rtn = execute_pipe_sequence(chldn->current->as_tree->children);
		else if (chldn->current->as_tree->type == TOKEN)
		{
			if (chldn->current->as_tree->token->type == TOK_AND)
			{
				if (first)
					rtn = execute_tok_and(chldn->current);
				first = false;
				if (rtn == 0)
					rtn = execute_pipe_sequence(chldn->current->next->as_tree->children);
				if (lnext(chldn) == NULL)
					break ;
			}
			else if (chldn->current->as_tree->token->type == TOK_OR)
			{
				if (first)
					rtn = execute_tok_or(chldn->current);
				first = false;
				if (rtn != 0)
					rtn = execute_pipe_sequence(chldn->current->next->as_tree->children);
				if (lnext(chldn) == NULL)
					break ;
			}
		}
	}
	set_last_exit_status(rtn);
	return (rtn);
}

#define EXIT_FORK_ERROR 4

pid_t	execute_complete_command_wrapper(t_tree *complete_command,
	t_list *commands)
{
	pid_t	pid;
	int		rtn;

	pid = fork();
	if (pid < 0)
		minishell_error(EXIT_FORK_ERROR, true, "%s", strerror(errno));
	else if (pid > 0)
		return (close_fds(complete_command), pid);
	set_fds(complete_command);
	close_other_command_fds(commands);
	rtn = execute_complete_command(complete_command);
	/* close_fds(complete_command); */ /* TODO: why doesn't this work? */
	finish(false);
	exit(rtn);
}

/* node should be the root of the ast */
/* TODO: think if 0 (success) is a sensible return for node == NULL */
unsigned char	execute(t_tree *node)
{
	unsigned char	rtn;

	if (node == NULL)
		return (0);
	rtn = execute_complete_command(node);
	return (rtn);
}
