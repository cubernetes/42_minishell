/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:01:02 by pgrussin          #+#    #+#             */
/*   Updated: 2024/02/15 19:47:52 by pgrussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/*dup lib*/
#include <unistd.h>
#include <stdio.h>

static void	execute_pipe_sequence(t_ast_node **commands)
{
	t_ast_node	**current_command;
	t_ast_node	**children;
	t_token		*redirection;
	t_token		*redir_file;

	current_command = commands;
	while (*current_command != NULL)
	{
		//can be del
		if ((*current_command)->type == SIMPLE_COMMAND)
		{
			children = (*current_command)->data.children;
			while (*children != NULL)
			{
				if ((*children)->type == IO_REDIRECT)
				{
					redirection = (*children)->data.children[0]->data.token;
					redir_file = (*children)->data.children[1]->data.token;
					//Implementation of TOK_WORD
					if (redirection->type == TOK_HEREDOC)
					{
						printf("HEREDOC_TOKEN: %s\n", redir_file->str);
					}
					if (redirection->type == TOK_OVERRIDE)
					{
						printf("OVERRIDE_TOKEN: %s\n", redir_file->str);
					}
					/* TODO: implement HEREDOC*/
					/* TODO: implement OVERRIDE*/
				}
				children++;
			}
		}
		current_command++;
	}
}

static void	execute_complete_command(t_ast_node *ast_node)
{
	int	i;

	i = 0;
	while (ast_node->data.children[i] != NULL)
	{
		printf("%d\n", i);
		if (ast_node->data.children[i]->type == PIPE_SEQUENCE)
			execute_pipe_sequence(ast_node->data.children[i]->data.children);
		i++;
	}
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	ast_node->meta_data.fds.in = dup(0);
	ast_node->meta_data.fds.out = dup(1);
	if (ast_node == NULL)
		return ;
	ast_print(ast_node);
	execute_complete_command(ast_node);
}
