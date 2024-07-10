/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 03:31:26 by paul              #+#    #+#             */
/*   Updated: 2024/07/10 19:25:03 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"
#include "libft.h"

#include <stddef.h>
#include <stdio.h>             /* needed for FILE typedef needed by readline */
#include <readline/history.h>  /* ... */
#include <readline/readline.h> /* readline() */
#include <stdlib.h>            /* exit(), DONT USE free or malloc! */
#include <string.h>
#include <unistd.h>            /* STDERR_FILENO */
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

/*TODO: what happens in nfinite loop in minishell with ctrl + c and cntr + d*/
int	minishell_error(int exit_code, bool do_exit,
bool syntax_error, const char *fmt, ...)
{
	va_list	ap;
	char	*errfmt;
	char	*err;

	if (option_enabled('i') && do_exit)
		ft_dprintf(STDERR_FILENO, "exit\n");
	va_start(ap, fmt);
	errfmt = ft_strjoin(var_lookup("0"), ": ");
	if (!option_enabled('i'))
	{
		if (option_enabled('c') && syntax_error)
			errfmt = ft_strjoin(errfmt, "-c: ");
		errfmt = ft_strjoin(errfmt, ft_strjoin("line ",
					ft_strjoin(var_lookup("LINENO"), ": ")));
	}
	if (fmt[0] != '!')
		errfmt = ft_replace_all(errfmt, "%", "%%");
	else
	{
		errfmt = "";
		++fmt;
	}
	err = ft_strjoin(errfmt, fmt);
	err = ft_strjoin(err, "\n");
	ft_vdprintf(STDERR_FILENO, err, ap);
	va_end(ap);
	if (!option_enabled('i') && syntax_error && fmt[0] != '!')
	{
		err = ft_strjoin(errfmt, "`%s'\n");
		ft_dprintf(STDERR_FILENO, err, var_lookup("CURRENT_LINE"));
	}
	if (do_exit)
	{
		finish(false);
		exit(exit_code);
	}
	return (exit_code);
}

char	*get_dollar_prompt(void)
{
	if (!ft_strcmp(ft_geteuid(), "0"))
		return ("#");
	else if (ft_geteuid()[0] == '\0')
		return ("?");
	return ("$");
}

/* TODO: Expand variables */
char	*expand_prompt(char *prompt_string)
{
	t_list	*replacements;

	replacements = lnew();
	lpush(replacements, as_str_pair(&(t_str_pair){"\\u", ft_getusername()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\w", get_cwd_for_prompt()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\W",
			lsplit(get_cwd_for_prompt(), "/")->last->as_str}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\h", ft_gethostname()}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\H",
			ft_split(ft_gethostname(), '.')[0]}));
	lpush(replacements, as_str_pair(&(t_str_pair){"\\$", get_dollar_prompt()}));
	liter(replacements);
	while (lnext(replacements))
		prompt_string = ft_replace_all(prompt_string,
				replacements->current->as_str_pair->l,
				uniquize(replacements->current->as_str_pair->l));
	liter(replacements);
	while (lnext(replacements))
		prompt_string = ft_replace_all(prompt_string,
				uniquize(replacements->current->as_str_pair->l),
				replacements->current->as_str_pair->r);
	return (prompt_string);
}

void	inherit_environment(char *envp[])
{
	t_list	*pair;

	while (*envp)
	{
		pair = lsplit_n(*envp, "=", 1);
		set_var(pair->first->as_str,
			pair->last->as_str,
			(t_flags){.exp = true});
		++envp;
	}
}

t_tree	*parse(char *line)
{
	t_list	*tokens;
	t_tree	*tree;

	tokens = tokenize(line);
	tree = build_ast(tokens, true);
	return (tree);
}
