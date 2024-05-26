#include "minishell.h"

#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>

static void	interactive_interupt(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_done = 1;
}

void	interactive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &interactive_interupt}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
}

void	noninteractive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = SIG_DFL}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_DFL}, NULL);
}
