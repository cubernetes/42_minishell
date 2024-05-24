#include "minishell.h"

#include <stdio.h>
#include <readline/readline.h>
#include <fcntl.h>
#include <unistd.h>

/* TODO: Check for error on write, open */
char	*create_heredoc(char *delimiter)
{
	char	*heredoc_file;
	char	*line;
	int		fd;

	heredoc_file = ft_mktemp("minishell.");
	fd = open(heredoc_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	while (1)
	{
		line = gc_add_str(readline(get_var("PS2")->value));
		if (line == NULL || !ft_strcmp(line, delimiter))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	if (line == NULL)
		minishell_error(0, false,
			"warning: here-document delimited by end-of-file (wanted `%s')",
			delimiter);
	return (heredoc_file);
}
