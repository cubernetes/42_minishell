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

/* TODO: Change name, since we're also setting vars */
int	add_var_flags(char key[static 1],
	char *value,
	t_declare_flags flags,
	t_var *orig_var)
{
	bool	readonly;
	bool	export;
	int		exit_status;

	readonly = orig_var && orig_var->readonly;
	export = (orig_var && orig_var->exp) || option_enabled('a');
	exit_status = 0;
	if (readonly && ft_nullable_strcmp(value, orig_var->value))
		exit_status = minishell_error(1, false, false,
			"declare: %s: readonly variable", key); // TODO: This is always "declare", is it?
	if (flags.not_readonly && readonly && exit_status == 0)
		exit_status = minishell_error(1, false, false,
			"declare: %s: readonly variable", key); // TODO: This is always "declare", is it?
	else if (flags.readonly && exit_status == 0)
		readonly = true;
	if (flags.not_export && exit_status == 0)
		export = false;
	else if (flags.export && exit_status == 0)
		export = true;
	set_var(key, value, (t_flags){
		.exp = export,
		.readonly = readonly,
		.special = (orig_var && orig_var->special)
	});
	return (exit_status);
}
/* Not doing readonly checking here! */
