/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:14 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:32:15 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	builtin_env(char **argv, t_fds fds)
{
	t_list		*vars;

	(void)argv;
	vars = liter(ht_to_list(get_vars()));
	while (lnext(vars))
		if (vars->current->as_kv_pair->v.as_var->exp
			&& !vars->current->as_kv_pair->v.as_var->special
			&& vars->current->as_kv_pair->v.as_var->value)
			ft_dprintf(fds.fd_out, "%s=%s\n",
				vars->current->as_kv_pair->k,
				vars->current->as_kv_pair->v.as_var->value);
	return (0);
}
