#include "libft.h"
#include "minishell.h"

bool	sort_vars(t_data data1, t_data data2)
{
	if (ft_strcmp(data1.as_kv_pair->k, data2.as_kv_pair->k) > 0)
		return (true);
	return (false);
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

t_flags	get_flags(char key[static 1])
{
	t_var	*var;

	var = get_var(key);
	if (var)
		return ((t_flags){
			.exp = var->exp,
			.readonly = var->readonly,
			.special = var->special,
			.unset = false,
		});
	else
		return ((t_flags){0});
}

void	add_var_flags(char key[static 1],
	char *value,
	t_declare_flags flags,
	t_var *orig_var)
{
	set_var(key, value, (t_flags){
		.exp = flags.export | (orig_var && orig_var->exp),
		.readonly = flags.readonly | (orig_var && orig_var->readonly),
		.special = (orig_var && orig_var->special)
	});
}
/* Not doing readonly checking here! */
