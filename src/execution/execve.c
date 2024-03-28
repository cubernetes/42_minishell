#include "../../include/minishell.h"
#include <unistd.h>

void	ms_execve(t_ast_node *command);
char	**get_environ(void);
char 	**get_argv(void);
void	ms_init(t_ast_node *simple_command)
{
	simple_command->meta_data.simple_command.exit_status = 0;
	simple_command->meta_data.simple_command.fds.in = -2;
	simple_command->meta_data.simple_command.fds.out = -2;
}
char **	ms_extract_path(char **envp)
{
	char	**path;
	int	cnt;

	cnt = -1;
	while (envp[++cnt])
	{
		if(!ft_memcmp(envp[cnt], "PATH=", 5))
		{
			path = ft_split(&envp[cnt][5], ':');
			return (path);
		}
	}
	return (NULL);
}


t_bool	execute_command(t_ast_node *command)
{
	pid_t	pid;
	t_bool	rtn;

	pid = fork();
	if (pid < 0)
		return(FALSE);
	if (pid > 0)
		return (TRUE);
	return (ms_execve(command), TRUE);
}

char *get_command(t_ast_node *comand)
{
	char	*rtn;

	rtn = 
}

void	ms_execve(t_ast_node *command)
{
	char	*tmp;
	char	**envp_path;
	int	i;
	char	*cmd;

	cmd = get_command(command);
	envp_path = ms_extract_path(get_environ());
	tmp = ft_strjoin("./", get_argv());
	if (!tmp)
		return ;
	i = 0;
	if (access(tmp, F_OK))
	{
		free(tmp);
		tmp = ft_strjoin(envp_path[i], command);
	}
}
