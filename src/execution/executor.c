/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:59:01 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/15 13:00:54 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_complete_command(t_ast_node *ast_node)
{
	(void)ast_node;
}

/* ast_node should be the root of the ast */
void	execute(t_ast_node *ast_node)
{
	execute_complete_command(ast_node);
}
