#include "minishell.h"

int	builtin_export(char **argv, t_fds fds)
{
	t_list	*key_value;

	if (*argv == NULL)
		builtin_env(argv, fds, "export ", true);
	while (*argv)
	{
		key_value = lsplit(*argv, "="); /* TODO, lsplit_one() */
		if (key_value->len == 1)
			continue ; /* TODO, handle vars without value */
		set_var(key_value->first->as_str, key_value->first->next->as_str, (t_flags){.exp = true});
		++argv;
	}
	return (0);
}
