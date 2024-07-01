#include "libft.h"
#include "minishell.h"

int	builtin_shift(char **argv, t_fds fds) // TODO: use fds
{
	int	num_args;
	int	idx;

	num_args = ft_atoi(var_lookup("#"));
	idx = 1;
	while (idx <= num_args)
	{
		set_var(ft_itoa(idx), var_lookup(ft_itoa(idx + 1)), get_flags(ft_itoa(idx)));
		++idx;
	}
	return (0);
}
