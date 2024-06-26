#include "minishell.h"

#include <stdio.h>
#include <readline/readline.h>
#include <fcntl.h>
#include <unistd.h>

bool	heredoc_aborted(int op)
{
	static bool	aborted = false;

	if (op == 1)
		aborted = true;
	else if (op == 0)
		aborted = false;
	return (aborted);
}

/* TODO: Check for error on write, open */
/* TOOD: heredoc in noninteractive mode */
char	*create_heredoc(char *delimiter)
{
	char		*heredoc_file;
	char		*input;
	t_fatptr	input_gnl;
	int			fd;
	char		*ps2;

	if (heredoc_aborted(-1))
		return ("");
	heredoc_file = ft_mktemp("minishell.");
	fd = open(heredoc_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	while (1)
	{
		if (shopt_enabled('i'))
		{
			ps2 = get_var("PS2")->value;
			interactive_signals();
			input = readline(ps2);
			noninteractive_signals();
			gc_add(input);
			if (input == NULL || !ft_strcmp(input, delimiter) || heredoc_aborted(-1))
				break ;
			write(fd, input, ft_strlen(input));
			write(fd, "\n", 1);
		}
		else
		{
			noninteractive_signals();
			input_gnl = get_next_fat_line(STDIN_FILENO);
			if (input_gnl.data == NULL || !ft_strcmp(input_gnl.data, delimiter) || heredoc_aborted(-1))
				break ;
			write(fd, input_gnl.data, input_gnl.size - 1);
			if (((char *)input_gnl.data)[input_gnl.size - 2] != '\n')
				write(fd, "\n", 1);
		}
	}
	close(fd);
	if (input == NULL)
		minishell_error(0, false,
			"warning: here-document delimited by end-of-file (wanted `%s')",
			delimiter);
	return (heredoc_file);
}
