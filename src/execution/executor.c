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
