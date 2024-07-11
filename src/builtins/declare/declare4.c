/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:22:32 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/11 18:41:20 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	declare_print_all(t_declare_flags flags, t_fds fds)
{
	t_list	*vars;

	vars = liter(lsort(ht_to_list(get_vars()), sort_vars));
	while (lnext(vars))
	{
		if (has_flag(vars->current->as_kv_pair->v.as_var, flags)
			&& !vars->current->as_kv_pair->v.as_var->special)
			print_var(vars->current->as_kv_pair, fds);
	}
	return (0);
}

int	declare_assign(t_declare_args args[static 1])
{
	int		exit_status;

	if (args->orig_var && args->orig_var->readonly)
	{
		exit_status = minishell_error(1, 0,
				"%s: %s: readonly variable", args->name, *args->key);
		*args->value = args->orig_var->value;
		add_var_flags(*args->key, *args->value, args->flags, args->orig_var);
		return (exit_status);
	}
	else if ((*args->key)[ft_strlen(*args->key) - 1] == '+' && args->orig_var
		&& args->orig_var->value)
		*args->value = ft_strjoin(args->orig_var->value, *args->value);
	if ((*args->key)[ft_strlen(*args->key) - 1] == '+')
		*args->key = ft_strndup(*args->key, ft_strlen(*args->key) - 1);
	exit_status = add_var_flags(*args->key, *args->value, args->flags,
			args->orig_var);
	return (exit_status);
}

static char	*mk_value(t_declare_args args[static 1])
{
	if (args->key_value->len <= 1)
		return ("");
	return (ft_strjoin("=", *args->value));
}

int	declare_set_helper(t_declare_args args[static 1])
{
	int		exit_status;

	exit_status = 0;
	if (((*args->key)[ft_strlen(*args->key) - 1] != '+'
		&& !valid_name(*args->key))
		|| ((*args->key)[ft_strlen(*args->key) - 1] == '+'
		&& !valid_name(ft_strndup(*args->key, ft_strlen(*args->key) - 1))))
		exit_status = minishell_error(1, 0,
				"%s: `%s': not a valid identifier", args->name,
				ft_strjoin(*args->key, mk_value(args)));
	else if (args->key_value->len == 2)
		exit_status = declare_assign(&(t_declare_args){args->orig_var,
				args->value, args->key, args->flags, args->name, NULL});
	else if (args->key_value->len == 1 && args->orig_var)
		exit_status = add_var_flags(*args->key, args->orig_var->value,
				args->flags, args->orig_var);
	else if (args->key_value->len == 1)
		exit_status = add_var_flags(*args->key, NULL, args->flags,
				args->orig_var);
	return (exit_status);
}
