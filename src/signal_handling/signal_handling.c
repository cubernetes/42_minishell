#include "../../libft/libft.h"
#include "../../include/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

static int	event(void)
{
	return (0);
}

static void	handle_sigint(int sig)
{
	(void)sig;
	rl_done = 1;
}

void	setup_signals(void)
{
	t_sa	sa_int;
	t_sa	sa_quit;

	rl_event_hook = event;
	sa_int = (t_sa){0};
	sa_quit = (t_sa){0};
	sa_int.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
