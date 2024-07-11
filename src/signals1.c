/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:07:26 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/11 15:44:14 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

void	interactive_interrupt(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_done = 1;
	set_last_exit_status(130);
}

void	noninteractive_interrupt(int sig)
{
	(void)sig;
	ft_printf("\n");
}

void	noninteractive_quit(int sig)
{
	(void)sig;
	ft_printf("Quit (core dumped)\n");
}

void	interactive_interrupt_heredoc(int sig)
{
	interactive_interrupt(sig);
	heredoc_aborted(1);
}

void	sigpipe_handler(int sig)
{
	(void)sig;
	rl_done = 1;
	finish(false);
	exit(141);
}
