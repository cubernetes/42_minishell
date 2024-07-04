/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cwd_for_prompt.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:52:49 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:52:54 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <unistd.h>

/* TODO: Update */
/** Return the current working directory, replacing the home directory
 *  (determined from the HOME environment variable) with a tilde ("~").
 *
 *  @returns: the current working directory where the home directory
 *  (determined from the HOME environment variable) is replaced with a tilde.
 */
char	*get_cwd_for_prompt(void)
{
	char		*cwd;
	char		*home_dir;

	if (get_var("PWD") == NULL)
	{
		cwd = gc_add_str(getcwd(NULL, 0));
		if (cwd == NULL)
			cwd = ".";
	}
	else
		cwd = var_lookup("PWD");
	home_dir = var_lookup("HOME");
	if (*home_dir && !ft_strncmp(home_dir, cwd, ft_strlen(home_dir)))
		cwd = ft_replace(cwd, home_dir, "~");
	return (cwd);
}
