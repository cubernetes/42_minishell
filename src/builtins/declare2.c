/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:56 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:32:05 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"
#include "minishell.h"

#include <unistd.h>

/* From declare_utils.c, normally static, but you know, max 5 functions... */
bool	sort_vars(t_data data1, t_data data2);
int		add_var_flags(char key[static 1],
			char *value,
			t_declare_flags flags,
			t_var *orig_var);
char	*flags_to_str(t_var *var);

/* bare_declare will be removed in the future when this function is
 * actually implemented
 */
char	*quote(char *s, bool bare_declare)
{
	if (bare_declare)
	{
		if (force_ansi_c_quoting(s))
			return (quote_ansi_c(s, bare_declare));
		else if (force_single_quoting(s))
			return (quote_single(s));
		return (s);
	}
	else
	{
		if (force_ansi_c_quoting(s))
			return (quote_ansi_c(s, bare_declare));
		return (quote_double(s));
	}
}

void	print_var(t_kv_pair *kv, t_fds fds)
{
	if (kv->v.as_var->value)
		ft_dprintf(fds.fd_out, "declare -%s %s=%s\n",
			flags_to_str(kv->v.as_var), kv->k,
			quote(kv->v.as_var->value, false));
	else
		ft_dprintf(fds.fd_out, "declare -%s %s\n",
			flags_to_str(kv->v.as_var), kv->k);
}

bool	valid_name(char *s)
{
	if (!ft_isalpha(*s) && !(*s == '_'))
		return (false);
	++s;
	while (*s)
	{
		if (!ft_isalnum(*s) && !(*s == '_'))
			return (false);
		++s;
	}
	return (true);
}

int	declare_print(char *name, char **argv, t_fds fds)
{
	int		exit_status;
	t_var	*var;

	exit_status = 0;
	while (*argv)
	{
		var = get_var(*argv);
		if (!valid_name(*argv))
			exit_status = minishell_error(1, false, false,
					"%s: `%s': not found", name, *argv);
		else if (var == NULL || var->special)
			exit_status = minishell_error(1, false, false,
					"%s: %s: not found", name, *argv);
		else
			print_var(&(t_kv_pair){.k = *argv, .v = as_var(var)}, fds);
		++argv;
	}
	return (exit_status);
}

bool	has_flag(t_var *var, t_declare_flags flags)
{
	if (flags.export && var->exp)
		return (true);
	if (flags.readonly && var->readonly)
		return (true);
	if (!(flags.export || flags.readonly))
		return (true);
	return (false);
}
