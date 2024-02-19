/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:01:02 by pgrussin          #+#    #+#             */
/*   Updated: 2024/02/15 20:50:34 by pgrussin         ###   ########.fr       */
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
static void	tok_override_redirection(t_token *redirection)
{
	
}
static void	tok_heredoc_redirection(t_token *redirection)
{
}

void	tok_input_redirection(t_token *redir_file, t_fdrd *redir_fd)
{
	if (redir_file->str != NULL)
		redir_fd->fdin = open(redir_file->str, O_RDONLY);
	else
		redir_fd->fdin = dup(redir_fd->tmp_fdin);
}

void check_redirection_token(t_token *redirection, t_token *redir_file, t_fdrd *redir_fd)
{
	if (redirection->type == TOK_HEREDOC)
		tok_heredoc_redirection(redir_file);
	if (redirection->type == TOK_OVERRIDE)
		tok_override_redirection(redir_file);
	if (redirection->type == TOK_INPUT)
		tok_input_redirection(redir_file, redir_fd);
}
void	iterate_pipe_sequence(t_ast_node **commands, t_fdrd *redir_fd)
{
	t_ast_node	**current_command;
	t_ast_node	**children;
	t_token		*redirection;
	t_token		*redir_file;

	current_command = commands;
	while (*current_command != NULL)
	{
		if ((*current_command)->type == SIMPLE_COMMAND)
		{
			children = (*current_command)->data.children;
			while (*children != NULL)
			{
				if ((*children)->type == IO_REDIRECT)
				{
					redirection = (*children)->data.children[0]->data.token;
					redir_file = (*children)->data.children[1]->data.token;
					check_redirection_token(redirection, redir_file, redir_fd);
				}
				children++;
			}
			dup2(redir_fd->fdin, 0);
			close(redir_fd->fdin);
		}
		current_command++;
	}
}

void	execute_pipe_sequence(t_ast_node **comands, t_fdrd *redir_fd)
{
	iterate_pipe_sequence(comands, redir_fd);
}
static void	execute_complete_command(t_ast_node *ast_node, t_fdrd *redir_fd)
{
	int	i;

	i = 0;
	while (ast_node->data.children[i] != NULL)
	{
		if (ast_node->data.children[i]->type == PIPE_SEQUENCE)
			execute_pipe_sequence(ast_node->data.children[i]->data.children, redir_fd);
		i++;
	}
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	t_fdrd *redir_fd;

	redir_fd = (struct t_fdrd *)ft_malloc(sizeof(t_fdrd));
	if (!redir_fd)
		return ;
	//ast_node->meta_data.fds.in = dup(0);
	//ast_node->meta_data.fds.out = dup(1);
	redir_fd->tmp_fdin = dup(0);
	redir_fd->tmp_fdout = dup(1);
	if (ast_node == NULL)
		return ;
	execute_complete_command(ast_node, redir_fd);
}
