/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:01:02 by pgrussin          #+#    #+#             */
/*   Updated: 2024/02/20 15:27:03 by pgrussin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
/*dup lib*/
#include <unistd.h>
/*open lib*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/*malloc lib*/
#include <stdlib.h>
/* TODO: delete, used for printf*/
#include <stdio.h>

/* TODO: check if fd redirection worked*/


/*int	iterate_pipe_sequence(t_ast_node **commands)
{
	t_ast_node	**current_command;
	t_ast_node	**children;
	t_token		*redirection;
	t_token		*redir_file;

	current_command = commands;
	while (*current_command != NULL)
	{
		if ((*current_command)->type == SIMPLE_COMMAND)
			handle_simple_comand();
		if ((*current_command)->type == COMPLETE_COMMAND)
			handle_complete_command();
		current_command++;
	}
}

int	execute_pipe_sequence(t_ast_node **comands)
{
	iterate_pipe_sequence(comands);
}*/

t_ast_node_type	give_ast_node_type(t_ast_node **ast_node)
{
	t_ast_node	**children;
	children = (*ast_node)->data.children;
	if (*children != NULL)
	{
		if ((*children)->type == COMPLETE_COMMAND)
			return (COMPLETE_COMMAND);
		if ((*children)->type == TOKEN)
			return (TOKEN);
		else if ((*children)->type == PIPE_SEQUENCE)
			return (PIPE_SEQUENCE);
		children++;
	}
	return (NULL);
}

int	execute_tok_and(t_ast_node **tok_and)
{
	printf("TOK AND: \n");
	t_ast_node	**child;

	child = (*tok_and)->data.children;
	if (give_ast_node_type(child) != NULL)
	{
		if (give_ast_node_type == PIPE_SEQUENCE)
			return (1);
	}
}

/*return value for TOK_AND & TOK_OR to see if command was succesful executet*/
int	execute_complete_command(t_ast_node *ast_node)
{
	int	i;
	int	rtn;
	t_ast_node **children;

	children = ast_node->data.children;
	while (*children != NULL)
	{
		/*if (ast_node->data.children[i]->type == PIPE_SEQUENCE)
			rtn = execute_pipe_sequence(ast_node->data.children[i]->data.children);
		if (ast_node->data.children[i]->type == SIMPLE_COMMAND)
			rtn = execute_simple_command(*child);*/
		if ((*children)->type == TOK_AND)
			rtn = execute_tok_and(*children);
		/*if (ast_node->data.children[i]->type == TOK_OR)
			rtn = execute_tok_or(*child);*/

		children++;
	}
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	/*t_fdrd *redir_fd;

	redir_fd = (struct t_fdrd *)ft_malloc(sizeof(t_fdrd));
	if (!redir_fd)
		return ;*/
	if (ast_node == NULL)
		return ;
	execute_complete_command(ast_node);
}
