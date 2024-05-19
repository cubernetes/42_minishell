#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <stdio.h>
#include <readline/readline.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

/* TODO: What to do in case of execve error? */
#define EXECVE_ERR 3
#define FORK_ERROR 4

void	close_fds(t_tree *simple_command)
{
	int	in;
	int	out;
	int	err;

	in = simple_command->fd_in;
	out = simple_command->fd_out;
	err = simple_command->fd_err;
	if (in != -2)
		close(in);
	if (out != -2)
		close(out);
	if (err != -2)
		close(err);
}

/* TODO: Replace magic number -2 with something like FD_UNINITIALIZED, ...*/
void	set_fds(t_tree *simple_command)
{
	int	in;
	int	out;
	int	err;

	in = simple_command->fd_in;
	out = simple_command->fd_out;
	err = simple_command->fd_err;
	if (in != -2)
		dup2(in, STDIN_FILENO);
	if (out != -2)
		dup2(out, STDOUT_FILENO);
	if (err != -2)
		dup2(err, STDERR_FILENO);
}

static char	*search_executable(char *program, char **path_parts)
{
	char	*path;
	char	*executable_path;

	if (ft_strchr(program, '/'))
		return (program);
	executable_path = NULL;
	while (program && *program && *path_parts)
	{
		path = ft_strjoin(*path_parts, "/");
		executable_path = ft_strjoin(path, program);
		if (!access(executable_path, R_OK)
			&& open(executable_path, O_DIRECTORY) == -1)
			break ;
		executable_path = NULL;
		++path_parts;
	}
	return (executable_path);
}

static char	**make_argv(t_tree *simple_command)
{
	char	**argv;
	t_list	*d_argv;
	int		i;

	d_argv = lnew();
	liter(simple_command->children);
	while (lnext(simple_command->children))
		if (simple_command->children->current->as_tree->type == TOKEN)
			lpush(d_argv, as_str(simple_command->children->current->as_tree->token->str));
	argv = ft_malloc(sizeof(*argv) * (d_argv->len + 1));
	i = 0;
	liter(d_argv);
	while (lnext(d_argv))
		argv[i++] = d_argv->current->as_str;
	argv[i] = NULL;
	return (argv);
}

void	close_other_command_fds(t_list *commands)
{
	liter(commands);
	while (lnext(commands))
		close_fds(commands->current->as_tree);
}

bool	is_builtin(char	*word)
{
	return (ft_strcmp(word, "cd") == 0
		|| ft_strcmp(word, "echo") == 0
		|| ft_strcmp(word, "env") == 0
		|| ft_strcmp(word, "exit") == 0
		|| ft_strcmp(word, "export") == 0
		|| ft_strcmp(word, "pwd") == 0
		|| ft_strcmp(word, "unset") == 0);
}

int	handle_builtin(char	**argv, t_fds fds)
{
	if (*argv == NULL)
		return (1);
	else if (ft_strcmp(*argv, "cd") == 0)
		return (builtin_cd(argv + 1, fds));
	else if (ft_strcmp(*argv, "echo") == 0)
		return (builtin_echo(argv + 1, fds));
	else if (ft_strcmp(*argv, "env") == 0)
		return (builtin_env(get_env(), fds));
	else if (ft_strcmp(*argv, "exit") == 0)
		return (builtin_exit(argv + 1, fds));
	else if (ft_strcmp(*argv, "export") == 0)
		return (builtin_export(argv + 1, fds));
	else if (ft_strcmp(*argv, "pwd") == 0)
		return (builtin_pwd(argv + 1, fds));
	else if (ft_strcmp(*argv, "unset") == 0)
		return (builtin_unset(argv + 1, fds));
	return (1);
}


/* TODO: Protect all system calls (dup2, fork, close, open, execve, ...) */
pid_t	execute_simple_command(t_tree *simple_command, t_list *commands)
{
	pid_t	pid;
	char	**path_parts;
	char	**argv;
	char	*program;

	path_parts = ft_split(env_lookup("PATH"), ':');
	argv = make_argv(simple_command);
	if (is_builtin(argv[0]))
		return (handle_builtin(argv, simple_command->fds) - 256);
	program = search_executable(argv[0], path_parts);
	if (!program)
		return (minishell_error(EXIT_COMMAND_NOT_FOUND, false, "%s: command not found",
			argv[0]), -1);
	pid = fork();
	if (pid < 0)
		minishell_error(FORK_ERROR, true, "%s", strerror(errno));
	if (pid > 0)
		return (close_fds(simple_command), pid);
	(set_fds(simple_command), close_other_command_fds(commands));
	execve(program, argv, get_env());
	(gc_free_all(), rl_clear_history()/*, clear_vars() */);
	return (minishell_error(EXECVE_ERR, false, "%s", strerror(errno)), -1);
}
