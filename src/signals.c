#include "minishell.h"

#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

unsigned char	get_last_exit_status(void)
{
	return (set_last_exit_status(-1));
}

unsigned char	set_last_exit_status(int new_exit_status)
{
	static unsigned char	exit_status = 0;

	if (new_exit_status >= 0)
	{
		exit_status = (unsigned char)new_exit_status;
		set_var("?", ft_itoa(exit_status), (t_flags){.special = true});
	}
	return (exit_status);
}

static void	interactive_interrupt(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_done = 1;
	set_last_exit_status(130);
}

static void	noninteractive_interrupt(int sig)
{
	(void)sig;
	ft_printf("\n");
}

static void	noninteractive_quit(int sig)
{
	(void)sig;
	ft_printf("Quit (core dumped)\n");
}

static void	interactive_interrupt_heredoc(int sig)
{
	interactive_interrupt(sig);
	heredoc_aborted(1);
}

void	interactive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &interactive_interrupt, .sa_flags = SA_RESTART}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
}

void	interactive_signals_heredoc(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &interactive_interrupt_heredoc}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
}

void	noninteractive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &noninteractive_interrupt, .sa_flags = SA_RESTART}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = &noninteractive_quit, .sa_flags = SA_RESTART}, NULL);
}
