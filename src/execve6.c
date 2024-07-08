
#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <stdio.h>
#include <readline/readline.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/* TODO: What to do in case of execve error? */
#define EXECVE_ERR 3
#define FORK_ERROR 4
/* TODO: Protect all system calls (dup2, fork, close, open, execve, ...) */
pid_t	execute_simple_command(t_tree *simple_command, t_list *commands)
{
	pid_t	pid;
	t_list	*path_parts;
	char	**argv;
	char	*program;
	int		exit_status;

	if (is_only_assigment_words(simple_command))
		argv = transform_for_declare(simple_command);
	else
		argv = make_argv(simple_command);
	if (var_lookup("PATH")[0] == '\0')
		path_parts = NULL;
	else
		path_parts = lsplit(var_lookup("PATH"), ":");
	if (option_enabled('x'))
		msh_xtrace(argv);
	if (argv[0] == NULL)
		return (close_fds(simple_command), -257);
	if (is_builtin(argv[0]) && simple_command->fd_in == -2 && simple_command->fd_out == -2)
		return (handle_builtin_wrapper(argv, simple_command) - 257);
	program = search_executable(argv[0], path_parts);
	if (!program && !is_builtin(argv[0]))
	{
		close_fds(simple_command);
		if (path_parts == NULL)
			minishell_error(EXIT_COMMAND_NOT_FOUND, false, false, "%s: No such file or directory", argv[0]);
		else
			minishell_error(EXIT_COMMAND_NOT_FOUND, false, false, "%s: command not found", argv[0]);
		if (simple_command->fd_in == -2 && simple_command->fd_out == -2) // only set underscore in foreground cmds
			set_underscore(argv);
		return (-1);
	}
	pid = fork();
	if (pid < 0)
		(close_fds(simple_command), minishell_error(FORK_ERROR, true, false, "%s", strerror(errno)));
	if (pid > 0)
		return (close_fds(simple_command), pid);
	set_var("_", get_argv()[0], (t_flags){0});
	set_fds(simple_command);
	close_other_command_fds(commands);
	if (is_builtin(argv[0]))
	{
		exit_status = handle_builtin_wrapper(argv, simple_command) - 256;
		finish(false);
		exit(exit_status);
	}
	execve(program, argv, get_env(program));
	exit_status = errno;
	if (open(program, O_DIRECTORY) != -1)
		minishell_error(EXECVE_ERR, false, false, "%s: %s", program, "Is a directory");
	else
		minishell_error(EXECVE_ERR, false, false, "%s: %s", program, strerror(exit_status));
	finish(false);
	if (exit_status == EACCES)
		exit(126);
	exit(127);
}