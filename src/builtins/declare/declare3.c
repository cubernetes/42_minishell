/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:59 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 20:09:46 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

#include <unistd.h>

int	declare_set(char *name, char **argv, t_declare_flags flags)
{
	t_list	*key_value;
	char	*key;
	char	*value;
	t_var	*orig_var;
	int		exit_status;

	exit_status = 0;
	while (*argv)
	{
		key_value = lsplit_n(*argv, "=", 1);
		key = key_value->first->as_str;
		value = key_value->first->next->as_str;
		if (key[ft_strlen(key) - 1] == '+')
			orig_var = get_var(ft_strndup(key, ft_strlen(key) - 1));
		else
			orig_var = get_var(key);
		if (!ft_strcmp(key, "") || !ft_strcmp(key, "+"))
			exit_status = minishell_error(1, 0,
					"%s: `%s': not a valid identifier", name, *argv);
		else
			exit_status = declare_set_helper(&(t_declare_args){orig_var,
					&value, &key, flags, name, key_value});
		++argv;
	}
	return (exit_status);
}

int	declare_print_set_vars(t_fds fds)
{
	t_list	*vars;

	vars = liter(lsort(ht_to_list(get_vars()), sort_vars));
	while (lnext(vars))
	{
		if (vars->current->as_kv_pair->v.as_var->value
			&& !vars->current->as_kv_pair->v.as_var->special)
			ft_dprintf(fds.fd_out, "%s=%s\n",
				vars->current->as_kv_pair->k,
				quote(vars->current->as_kv_pair->v.as_var->value, true));
	}
	return (0);
}

t_list	*get_opts(char *const argv[])
{
	t_list			*opts;
	char			erropt;

	if (!ft_strcmp(argv[0], "export"))
	{
		opts = ft_getopt(argv, "p", &erropt, &optind);
		lpush(opts, as_getopt_arg('x'));
	}
	else if (!ft_strcmp(argv[0], "readonly"))
	{
		opts = ft_getopt(argv, "p", &erropt, &optind);
		lpush(opts, as_getopt_arg('r'));
	}
	else
		opts = ft_getopt_plus(argv, "pxr", &erropt, &optind);
	liter(opts);
	if (erropt)
		return (llast(opts), minishell_error(1, 0,
				"%s: -%c: invalid option", argv[0], erropt), NULL);
	return (opts);
}

void	set_declare_flags(t_list *opts, t_declare_flags *flags)
{
	while (lnext(opts))
	{
		if ((char)opts->current->as_getopt_arg == 'p')
			flags->print = true;
		if ((char)opts->current->as_getopt_arg == 'r')
		{
			if (opts->current->as_getopt_arg & 1 << 8)
				flags->not_readonly = true;
			else
				flags->readonly = true;
		}
		if ((char)opts->current->as_getopt_arg == 'x')
		{
			if (opts->current->as_getopt_arg & 1 << 8)
				flags->not_export = true;
			else
				flags->export = true;
		}
	}
}

int	builtin_declare(char **argv, t_fds fds)
{
	t_list			*opts;
	t_declare_flags	flags;

	opts = get_opts(argv);
	if (opts == NULL)
		return (2);
	flags = (struct s_declare_flags){0};
	set_declare_flags(opts, &flags);
	if (flags.print && argv[optind] != NULL)
		return (declare_print(argv[0], argv + optind, fds));
	else if (flags.print || (argv[optind] == NULL
			&& (flags.export || flags.readonly)))
		return (declare_print_all(flags, fds));
	else if (argv[optind] == NULL)
		return (declare_print_set_vars(fds));
	else
		return (declare_set(argv[0], argv + optind, flags));
}

/* Cannot use `+' for removing attributes */
