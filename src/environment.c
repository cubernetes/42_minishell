/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 02:48:15 by tosuman           #+#    #+#             */
/*   Updated: 2024/06/26 18:02:45 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

char	**set_argv(char **argv)
{
	static char	**_argv = NULL;

	if (argv)
		_argv = argv;
	return (_argv);
}

char	**get_argv(void)
{
	return (set_argv(NULL));
}

char	**get_env(char *program)
{
	t_list		*vars;
	t_list		*env_vars;
	char		**envp;

	vars = liter(ht_to_list(get_vars()));
	env_vars = lnew();
	while (lnext(vars))
		if (vars->current->as_kv_pair->v.as_var->exp
			&& !vars->current->as_kv_pair->v.as_var->special
			&& vars->current->as_kv_pair->v.as_var->value
			&& ft_strcmp(vars->current->as_kv_pair->v.as_var->value, "_"))
			lpush(env_vars, as_data(vars->current));
	// ft_dprintf(fds.fd_out, "%s=%s\n",
	// 	vars->current->as_kv_pair->k,
	// 	vars->current->as_kv_pair->v.as_var->value);
	envp = ft_malloc(sizeof(*envp) * (env_vars->len + 2));
	envp[env_vars->len] = ft_strjoin("_=", program);
	envp[env_vars->len + 1] = NULL;
	liter(env_vars);
	while (lnext(env_vars))
		envp[env_vars->current_idx] = ft_strjoin(ft_strjoin(env_vars->current->as_kv_pair->k, "="), env_vars->current->as_kv_pair->v.as_var->value);
	return (envp);
}
