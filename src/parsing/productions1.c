/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   productions1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:36:10 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 18:51:30 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_tree	gen_production(char *token_str)
{
	return (*(t_tree *)ft_memdup(\
		&(t_tree){
			.type = tree_ht_get(token_str),
			.token = ft_memdup(
				&(t_token){\
					.type = tokens_ht_get(token_str), \
					.str = "", \
					.is_last_token = true \
				}, \
				sizeof(t_token) \
			), \
			0, \
			false, \
			{{0}} \
		},
		sizeof(t_tree)
	));
}

t_tree	**initialize_productions(const char *grammar)
{
	t_tree	**productions;
	char	**lines;
	char	**tokens;
	int		i;
	int		j;

	gc_start_context("POST");
	productions = ft_malloc(sizeof(*productions) * 20);
	lines = ft_split(grammar, '\n');
	i = -1;
	while (lines[++i])
	{
		tokens = ft_split(lines[i], ' ');
		j = 0;
		while (tokens[j])
			++j;
		productions[i] = ft_malloc(sizeof(*productions[i]) * (size_t)(j + 1));
		j = -1;
		while (tokens[++j])
			productions[i][j] = gen_production(tokens[j]);
		productions[i][j] = (t_tree){0};
	}
	gc_end_context();
	return (productions);
}

static char	*_get_grammar(void)
{
	static char	*grammar = \
		"<pipe_sequence> <complete_command_tail>"			"\n" \
		"TOK_EPSILON"										"\n" \
		"<and_or> <pipe_sequence> <complete_command_tail>"	"\n" \
		"&&"												"\n" \
		"||"												"\n" \
		"<command> <pipe_sequence_tail>"					"\n" \
		"TOK_EPSILON"										"\n" \
		"| <command> <pipe_sequence_tail>"					"\n" \
		"<simple_command>"									"\n" \
		"<compound_command>"								"\n" \
		"( <complete_command> )"							"\n" \
		"<io_redirect> TOK_WORD <simple_command_tail>"		"\n" \
		"TOK_EPSILON"										"\n" \
		"<io_redirect> TOK_WORD <simple_command_tail>"		"\n" \
		"TOK_EPSILON"										"\n" \
		">>" "\n"	"<<" "\n"	">" "\n"	"<" "\n"			\
		"TOK_EPSILON";

	return (grammar);
}

/* TODO: Don't use 0 as NULL */
/* tabsize: 4 */
t_tree	*get_production(t_tree_type nonterm, t_token *token)
{
	static t_tree		**productions = NULL;
	int					idx;

	if (productions == NULL)
		productions = initialize_productions(_get_grammar());
	idx = get_production_idx(nonterm, token);
	if (idx != -1)
		return (productions[idx]);
	return (NULL);
}
