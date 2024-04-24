#include "libft.h"
#include <readline/readline.h>
#define _POSIX_C_SOURCE 200809L
#include "../../include/minishell.h"
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

/* TODO: What to do in case of execve error? */
#define EXIT_EXEVE_ERROR 3
#define EXIT_FORK_ERROR 4

void	close_fds(t_ast_node *simple_command)
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
void	set_fds(t_ast_node *simple_command)
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

static char	**make_argv(t_ast_node *simple_command)
{
	t_di	*di;
	char	**argv;
	t_deque	*d_argv;
	int		i;

	d_argv = deque_init();
	di = di_begin(simple_command->children);
	while (di_next(di))
		if (di_get(di)->as_ast_node->type == TOKEN)
			deque_push_ptr_right(d_argv, di_get(di)->as_ast_node->token->str);
	argv = ft_malloc(sizeof(*argv) * (d_argv->size + 1));
	i = 0;
	di = di_begin(d_argv);
	while (di_next(di))
		argv[i++] = di_get(di)->as_str;
	argv[i] = NULL;
	return (argv);
}

void	close_other_command_fds(t_deque *commands)
{
	t_di	*di;

	di = di_begin(commands);
	while (di_next(di))
		close_fds(di_get(di)->as_ast_node);
}

/* TODO: Protect all system calls (dup2, fork, close, open, execve, ...) */
pid_t	execute_simple_command(t_ast_node *simple_command, t_deque *commands)
{
	pid_t	pid;
	char	**path_parts;
	char	**argv;
	char	*program;

	path_parts = ft_split(env_lookup("PATH"), ':');
	argv = make_argv(simple_command);
	program = search_executable(argv[0], path_parts);
	if (!program)
	{
		minishell_error(EXIT_COMMAND_NOT_FOUND, FALSE, "%s: command not found",
			argv[0]);
		return (-1);
	}
	pid = fork();
	if (pid < 0)
		minishell_error(EXIT_FORK_ERROR, TRUE, "%s", strerror(errno));
	if (pid > 0)
		return (close_fds(simple_command), pid);
	set_fds(simple_command);
	close_other_command_fds(commands);
	execve(program, argv, get_env());
	(gc_free(), rl_clear_history());
	minishell_error(EXIT_EXEVE_ERROR, FALSE, "%s", strerror(errno));
	return (-1);
}
