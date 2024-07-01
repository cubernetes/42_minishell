#include "minishell.h"

t_tree	*new_tree_token(t_token_type type, char *str)
{
	return (ft_memdup(\
		&(t_tree){
			.type = TOKEN,
			{.token = new_token(str, type, true)},
			0,
			false,
			{{0}}
		},
		sizeof(t_tree)
	));
}

t_tree	*new_tree_m_token(t_token_type type, const char *str)
{
	return (new_tree_token(type, ft_strdup(str)));
}

/* children must be allocated on heap: Update, maybe outdated */
t_tree	*new_tree_nonterm(t_tree_type type, t_list *children)
{
	return (ft_memdup(\
		&(t_tree){
			.type = type,
			.children = children
		},
		sizeof(t_tree)
	));
}
