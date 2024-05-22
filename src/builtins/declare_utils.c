#include "libft.h"
#include "minishell.h"

bool	sort_vars(t_data data1, t_data data2)
{
	return (!ft_strcmp(data1.as_str, data2.as_str));
}

bool	q_impl_p(bool p, bool q)
{
	return (!q || p);
}

char	*flags_to_str(t_var *var)
{
	char	*flags;

	flags = "";
	if (var->readonly)
		flags = ft_strjoin(flags, "r");
	if (var->exp)
		flags = ft_strjoin(flags, "x");
	if (flags[0] == '\0')
		flags = "-";
	return (flags);
}

void	add_var_flags(char key[static 1],
	char value[static 1],
	t_declare_flags flags,
	t_var orig_var[static 1])
{
	set_var(key, value, (t_flags){
		.exp = flags.export | (orig_var && orig_var->exp),
		.readonly = flags.readonly | (orig_var && orig_var->readonly),
		.special = (orig_var && orig_var->special)
	});
}
/* Not doing readonly checking here! */
