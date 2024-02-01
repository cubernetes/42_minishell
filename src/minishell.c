/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/01 09:46:15 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include "../include/memory.h"
#include <stdio.h> /* needed for FILE typedef needed by readline */
#include <readline/history.h> /* ... */
#include <readline/readline.h> /* readline() */
#include <unistd.h> /* STDERR_FILENO */

/* TODO: should not be used anywhere for submission!!! */
void	internal_error(const char *err, int line_no)
{
	ft_dprintf(STDERR_FILENO, "\033[31mLine %d: %s\033[m\n", line_no, err);
	exit(42);
}

/* TODO: NOT REQUIRED: add basic prompt expansion */
char	*expand_prompt(const char *prompt_string)
{
	return (ft_strdup(prompt_string));
}

void	update_state(t_state *state)
{
	state->ps0 = expand_prompt(PS0);
	state->ps1 = expand_prompt(PS1);
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	(void)ast_node;
}

t_bool	free_datastructures(char **line, t_ddeque **tokens,
	t_ast_node **ast_node)
{
	((void)ddeque_free(*tokens, free_token), *tokens = NULL);
	((void)ast_free(*ast_node), *ast_node = NULL);
	return (TRUE);
}

t_bool	free_state(t_state *state, char **line, t_ddeque **tokens,
	t_ast_node **ast_node)
{
	(void)free_datastructures(line, tokens, ast_node);
	return (TRUE);
}

/* TODO: what if readline returns NULL? */
/* TODO: use/think about rl_end (and other rl vars) */
/* TODO: remove DEBUG macros */
/* TODO: remove ddeque_print(tokens, print_token); */
/* TODO: glob expansion, env var expansion, and quote handling */
/* TOOD: parsing */
/* TODO: execution */
/* TODO: signal handling */
/* TODO: protect all mallocs (e.g. xmalloc(size_t n, int lineno), etc.) */
/* TODO: check for NULL when using any function that returns a malloced ptr */
/* TODO: make functions static where needed */
int	main(void)
{
	static t_state	state;
	static int		i = 10;
	char			*line;
	t_ddeque		*tokens;
	t_ast_node		*ast_root_node;

	tokens = NULL;
	ast_root_node = NULL;
	setup_signals();
	while (i--)
	{
		update_state(&state);
		line = readline(state.ps1);
		if (!line && free_state(&state, &line, &tokens, &ast_root_node))
			break ;
		tokens = tokenize(line);
		ddeque_print(tokens, print_token);
		ft_printf("\n");
		ast_root_node = build_ast(tokens);
		ast_print(ast_root_node);
		execute(ast_root_node);
		(void)free_state(&state, &line, &tokens, &ast_root_node);
	}
	free_all_ptrs();
	return (0);
}
