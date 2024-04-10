#include "../../libft/libft.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

typedef struct sigaction	t_sa;

static void	handle_sigint(int sig)
{
	(void)sig;
	ft_putchar('\n');
	rl_replace_line("", 0);
	rl_on_new_line();
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
