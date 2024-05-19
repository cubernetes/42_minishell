#include "libft.h"

t_data	as_token_type(t_token_type as_token_type)
{
	return ((t_data){.as_token_type = as_token_type, .type = TYPE_TOKEN_TYPE});
}

t_data	as_tree_type(t_tree_type as_tree_type)
{
	return ((t_data){.as_tree_type = as_tree_type, .type = TYPE_TREE_TYPE});
}
