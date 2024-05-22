#include "libft.h"
#include "minishell.h"

int	builtin_export(char **argv, t_fds fds)
{
	t_list	*key_value;
	char	*key;
	char	*value;
	t_var	*orig_var;
	int		exit_status;

	exit_status = 0;
	if (*++argv == NULL)
		builtin_declare((char *[]){"declare", "-px", NULL}, fds);
	// while (*argv)
	// {
	// 	key_value = lsplit_n(*argv, "=", 1);
	// 	key = key_value->first->as_str;
	// 	value = key_value->first->next->as_str;
	// 	orig_var = get_var(key);
	// 	if (key_value->len == 2)
	// 	{
	// 		if (orig_var->readonly)
	// 		{
	// 			exit_status = minishell_error(1, false, "%s: readonly variable",
	// 					key);
	// 			value = orig_var->value;
	// 		}
	// 		set_var(key, value, (t_flags){.exp = true,
	// 			.special = orig_var->special, .readonly = orig_var->readonly});
	// 	}
	// 	else if (key_value->len == 1)
	// 		set_var(key, orig_var->value, (t_flags){.exp = true,
	// 			.readonly = orig_var->readonly, .special = orig_var->special});
	// 	++argv;
	// }
	return (exit_status);
}
