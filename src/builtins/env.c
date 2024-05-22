#include "libft.h"
#include "minishell.h"

int	builtin_env(char **argv, t_fds fds)
{
	t_list		*vars;

	vars = liter(ht_to_list(get_vars()));
	while (lnext(vars))
		if (vars->current->as_kv_pair->v.as_var->exp)
			ft_printf("%s=%s\n",
				vars->current->as_kv_pair->k,
				vars->current->as_kv_pair->v.as_var->value);
	return (0);
}
