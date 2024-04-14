#include "../../libft/libft.h"
#include "../../include/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

static void	handle_sigint(int sig)
{
	(void)sig;
	/* rl_on_new_line(); */
	ft_putchar('\n');
	ft_printf("%s", rl_prompt);
	rl_replace_line("", 1);
	rl_redisplay();
}

void	setup_signals(void)
{
	t_sa	sa_int;
	t_sa	sa_quit;

	sa_int = (t_sa){0};
	sa_quit = (t_sa){0};
	sa_int.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
