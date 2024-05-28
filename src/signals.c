#include "minishell.h"

#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

static void	interactive_interupt(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_done = 1;
}

static void	noninteractive_interrupt(int sig)
{
	(void)sig;
	ft_printf("\n");
}

static void	interactive_interupt_heredoc(int sig)
{
	interactive_interupt(sig);
	heredoc_aborted(1);
}

void	interactive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &interactive_interupt, .sa_flags = SA_RESTART}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
}

void	interactive_signals_heredoc(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &interactive_interupt_heredoc}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
}

void	noninteractive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &noninteractive_interrupt, .sa_flags = SA_RESTART}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
}
