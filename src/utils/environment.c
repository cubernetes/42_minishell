/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 02:48:15 by tosuman           #+#    #+#             */
/*   Updated: 2024/04/01 02:53:52 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**set_env(char **envp)
{
	static char	**_envp = NULL;

	if (envp)
		_envp = envp;
	return (_envp);
}

char	**get_env(void)
{
	return (set_env(NULL));
}

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

char	*env_lookup(char *var)
{
	char	**envp;
	size_t	len;

	envp = get_env();
	while (envp && envp[0])
	{
		len = ft_strlen(var);
		if (!ft_strncmp(envp[0], var, len)
			&& envp[0][len] == '=')
			return (envp[0] + len + 1);
		++envp;
	}
	return ("");
}
