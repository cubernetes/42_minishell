/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paul <paul@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:53 by paul              #+#    #+#             */
/*   Updated: 2024/07/11 18:25:34 by tosuman          ###   ########.fr       */
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

bool	force_ansi_c_quoting(char *s)
{
	static const char	*force_ansi_c_quoting = FORCE_ANSI_C_QUOTING1 \
											FORCE_ANSI_C_QUOTING2 \
											FORCE_ANSI_C_QUOTING3;

	while (*s)
		if (ft_strchr(force_ansi_c_quoting, *s++))
			return (true);
	return (false);
}

bool	force_single_quoting(char *s)
{
	while (*s)
		if (ft_strchr(FORCE_SINGLE_QUOTING, *s++))
			return (true);
	return (false);
}

char	*quote_single(char *s)
{
	char	*ret;

	ret = "'";
	while (*s)
	{
		if (*s == '\'')
			ret = ft_strjoin(ret, "'\"'\"'");
		else
			ret = ft_strjoin(ret, ft_strndup(s, 1));
		++s;
	}
	return (ft_strjoin(ret, "'"));
}
/* Runs in quadratic time... */

char	*quote_double(char *s)
{
	char	*ret;

	ret = "\"";
	while (*s)
	{
		if (*s == '$')
			ret = ft_strjoin(ret, "\\$");
		else if (*s == '"')
			ret = ft_strjoin(ret, "\\\"");
		else if (*s == '\\')
			ret = ft_strjoin(ret, "\\\\");
		else
			ret = ft_strjoin(ret, ft_strndup(s, 1));
		++s;
	}
	return (ft_strjoin(ret, "\""));
}
/* Rules are adapted for minishell, not bash, so ` is not escaped */

/* TODO: Remove bare declare param */
char	*quote_ansi_c(char *s, bool bare_declare)
{
	if (bare_declare)
		return (quote_single(s));
	return (quote_double(s));
}
/* bare_declare will be removed in the future when this function is
 * actually implemented
 */
