/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:30:56 by tosuman           #+#    #+#             */
/*   Updated: 2024/07/11 18:40:57 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	get_key_value(char **key, char **value, t_list **key_value,
	char *assign)
{
	*key_value = lsplit_n(assign, "=", 1);
	*key = (*key_value)->first->as_str;
	if ((*key_value)->len > 1)
		*value = (*key_value)->first->next->as_str;
	else
		*value = ft_strdup("");
}

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
		get_key_value(&key, &value, &key_value, *argv);
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
