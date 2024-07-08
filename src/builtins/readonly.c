/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readonly.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:32:27 by paul              #+#    #+#             */
/*   Updated: 2024/07/08 03:32:28 by paul             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	builtin_readonly(char **argv, t_fds fds)
{
	char	**args;
	int		i;

	if (argv[1] == NULL)
		return (builtin_declare((char *[]){"declare", "-pr", NULL}, fds));
	args = ft_malloc(sizeof(*args) * (3 + ft_arrlen(argv)));
	args[0] = "readonly";
	i = 0;
	if (ft_strcmp(argv[i], "--"))
		++argv;
	while (argv[++i])
		args[i] = argv[i];
	args[i] = NULL;
	return (builtin_declare(args, fds));
}
