/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/24 19:56:48 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void	update_state(t_state *state)
{
	state->ps0 = ft_strdup("> ");
	state->ps1 = ft_strdup("$ ");
}

void	free_state(t_state *state)
{
	free(state->ps0);
	free(state->ps1);
}

int	main(void)
{
	char			*line;
	static t_state	state;

	while (1)
	{
		update_state(&state);
		line = readline(state.ps1);
		free_state(&state);
	}
	(void)line;
	return (0);
}
