/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:07:47 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/11 15:42:17 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	interactive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &interactive_interrupt,
		.sa_flags = SA_RESTART}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGPIPE, &(t_sa){.sa_handler = &sigpipe_handler}, NULL);
}

void	interactive_signals_heredoc(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &interactive_interrupt_heredoc},
		NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGPIPE, &(t_sa){.sa_handler = &sigpipe_handler}, NULL);
}

void	noninteractive_signals(void)
{
	sigaction(SIGINT, &(t_sa){.sa_handler = &noninteractive_interrupt,
		.sa_flags = SA_RESTART}, NULL);
	sigaction(SIGQUIT, &(t_sa){.sa_handler = &noninteractive_quit,
		.sa_flags = SA_RESTART}, NULL);
	sigaction(SIGPIPE, &(t_sa){.sa_handler = &sigpipe_handler}, NULL);
}
