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

void	close_fds(t_tree *command)
{
	int	in;
	int	out;
	int	err;

	in = command->fd_in;
	out = command->fd_out;
	err = command->fd_err;
	if (in != -2)
		close(in);
	if (out != -2)
		close(out);
	if (err != -2)
		close(err);
	command->fd_in = -2;
	command->fd_out = -2;
	command->fd_err = -2;
}

/* TODO: Replace magic number -2 with something like FD_UNINITIALIZED, ...*/
void	set_fds(t_tree *command)
{
	int	in;
	int	out;
	int	err;

	in = command->fd_in;
	out = command->fd_out;
	err = command->fd_err;
	if (in != -2)
		dup2(in, STDIN_FILENO);
	if (out != -2)
		dup2(out, STDOUT_FILENO);
	if (err != -2)
		dup2(err, STDERR_FILENO);
}

static char	*search_executable(char *program, t_list *path_parts) // TODO: Not correct, not searching for executables, maybe use stat instead
{
	char	*path;
	char	*executable_path;

	if (!program || !*program)
		return (NULL);
	if (ft_strchr(program, '/'))
		return (program);
	executable_path = NULL;
	liter(path_parts);
	while (lnext(path_parts))
	{
		if (*path_parts->current->as_str == '\0')
			path_parts->current->as_str = ".";
		path = ft_strjoin(path_parts->current->as_str, "/");
		executable_path = ft_strjoin(path, program);
		if (!access(executable_path, X_OK)
			&& open(executable_path, O_DIRECTORY) == -1)
			break ;
		executable_path = NULL;
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
	// if (i > 0)
	// 	set_var("_", argv[i - 1], (t_flags){0});
	return (argv);
}

void	close_other_command_fds(t_list *commands)
{
	liter(commands);
	while (lnext(commands))
		close_fds(commands->current->as_tree);
}

/* deliberately disable env, since wtf, env should NOT be a builtin. */
/* pedago fu'ed up here. what if you want to check if _ is working? CAN'T DO IT */
/* quoteth the bible (POSIX): "The env UTILITY shall obtain the current environment" */
		/* || ft_strcmp(word, "env") == 0 */
bool	is_builtin(char	*word)
{
	return (ft_strcmp(word, "cd") == 0
		|| ft_strcmp(word, "echo") == 0
		|| ft_strcmp(word, "exit") == 0
		|| ft_strcmp(word, "export") == 0
		|| ft_strcmp(word, "pwd") == 0
		|| ft_strcmp(word, "readonly") == 0
		|| ft_strcmp(word, "declare") == 0
		|| ft_strcmp(word, "source") == 0
		|| ft_strcmp(word, ".") == 0
		|| ft_strcmp(word, ":") == 0
		|| ft_strcmp(word, "shift") == 0
		|| ft_strcmp(word, "unset") == 0);
}

int	handle_builtin(char	*argv[], t_fds fds)
{
	if (*argv == NULL)
		return (1);
	else if (ft_strcmp(*argv, "cd") == 0)
		return (builtin_cd(argv, fds));
	else if (ft_strcmp(*argv, "echo") == 0)
		return (builtin_echo(argv, fds));
	else if (ft_strcmp(*argv, "env") == 0)
		return (builtin_env(argv, fds));
	else if (ft_strcmp(*argv, "exit") == 0)
		return (builtin_exit(argv, fds));
	else if (ft_strcmp(*argv, "export") == 0)
		return (builtin_export(argv, fds));
	else if (ft_strcmp(*argv, "pwd") == 0)
		return (builtin_pwd(argv, fds));
	else if (ft_strcmp(*argv, "readonly") == 0)
		return (builtin_readonly(argv, fds));
	else if (ft_strcmp(*argv, "declare") == 0)
		return (builtin_declare(argv, fds));
	else if (ft_strcmp(*argv, "unset") == 0)
		return (builtin_unset(argv, fds));
	else if (ft_strcmp(*argv, "source") == 0)
		return (builtin_source(argv, fds));
	else if (ft_strcmp(*argv, ".") == 0)
		return (builtin_source(argv, fds));
	else if (ft_strcmp(*argv, ":") == 0)
		return (builtin_colon(argv, fds));
	else if (ft_strcmp(*argv, "shift") == 0)
		return (builtin_shift(argv, fds));
	return (1);
}

static void	set_underscore(char *const argv[])
{
	if (*argv == NULL)
		return ;
	while (*argv)
		++argv;
	--argv;
	set_var("_", *argv, (t_flags){0});
}

int	handle_builtin_wrapper(char	*argv[], t_tree *simple_command)
{
	int	exit_status;
	int	orig_in;
	int	orig_out;
	int	orig_err;

	orig_in = simple_command->fd_in;
	orig_out = simple_command->fd_out;
	orig_err = simple_command->fd_err;
	if (simple_command->fd_in == -2)
		simple_command->fd_in = STDIN_FILENO;
	if (simple_command->fd_out == -2)
		simple_command->fd_out = STDOUT_FILENO;
	if (simple_command->fd_err == -2)
		simple_command->fd_err = STDERR_FILENO;
	exit_status = handle_builtin(argv, simple_command->fds);
	if (orig_in != -2)
		close(orig_in);
	if (orig_out != -2)
		close(orig_out);
	if (orig_err != -2)
		close(orig_err);
	simple_command->fd_in = -2;
	simple_command->fd_out = -2;
	simple_command->fd_err = -2;
	set_underscore(argv);
	return (exit_status);
}

static void	msh_xtrace(char *const argv[])
{
	t_list	*args;
	char	*xtrace_fd_str;
	int		xtrace_fd;

	args = lnew();
	while (*argv)
	{
		if (force_ansi_c_quoting(*argv))
			lpush(args, as_str(quote_ansi_c(*argv, false)));
		else if (force_single_quoting(*argv))
			lpush(args, as_str(quote_single(*argv)));
		else
			lpush(args, as_str(*argv));
		++argv;
	}
	xtrace_fd_str = var_lookup("MINISHELL_XTRACEFD");
	if (xtrace_fd_str[0] == '\0')
		xtrace_fd_str = ft_itoa(MINISHELL_DEFAULT_XTRACEFD);
	xtrace_fd = ft_atoi(xtrace_fd_str);
	ft_dprintf(xtrace_fd, ft_strjoin(var_lookup("PS4"), ljoin(args, " ")));
	ft_dprintf(xtrace_fd, "\n");
}

static char	**transform_for_declare(t_tree *simple_command)
{
	char	**argv;
	t_list	*d_argv;
	int		i;

	d_argv = lnew();
	lpush(d_argv, as_str("declare"));
	lpush(d_argv, as_str("--"));
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
	// if (i > 0)
	// 	set_var("_", argv[i - 1], (t_flags){0});
	return (argv);
}

static bool	is_only_assigment_words(t_tree *simple_command)
{
	t_list	*split_token;
	char	*word;
	int		idx;
	t_token	*token;
	int		assignment_words;

	assignment_words = 0;
	liter(simple_command->children);
	while (lnext(simple_command->children))
	{
		if (simple_command->children->current->as_tree->type == TOKEN)
		{
			token = simple_command->children->current->as_tree->token;
			split_token = lsplit(token->str, "=");
			if (split_token->len > 1)
			{
				word = split_token->first->as_str;
				idx = -1;
				while (word[++idx]) // word is always shorter than escape_ctx, quoting_ctx, and expansion_ctx
				{
					if (token->escape_ctx[idx] == '1')
						return (false);
					else if (token->quoting_ctx[idx] == '1')
						return (false);
					else if (token->expansion_ctx[idx] == '1')
						return (false);
					else if (ft_isalnum(word[idx]) || word[idx] == '_')
					{
						if (idx == 0 && ft_isdigit(word[idx]))
							return (false);
					}
					else
						return (false);
				}
				++assignment_words;
			}
			else
				return (false);
		}
	}
	return ((bool)assignment_words);
}

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
	path_parts = lsplit(var_lookup("PATH"), ":"); // TOOD: what about empty PATH
	if (option_enabled('x'))
		msh_xtrace(argv);
	if (argv[0] == NULL)
		return (close_fds(simple_command), -257);
	if (is_builtin(argv[0]) && simple_command->fd_in == -2 && simple_command->fd_out == -2)
		return (handle_builtin_wrapper(argv, simple_command) - 257);
	program = search_executable(argv[0], path_parts);
	if (!program && !is_builtin(argv[0]))
	{
		/*close_fds(simple_command),*/
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
