/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 20:08:09 by tosuman           #+#    #+#             */
/*   Updated: 2024/07/05 00:22:16 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_getopt.h"

#include <stdlib.h>

static void	init(t_list **opts, int *i, char **erropt)
{
	if (*erropt == NULL)
		*erropt = ft_malloc(sizeof(char));
	*opts = lnew();
	*i = 0;
	**erropt = '\0';
}

static bool	parse_arg(t_args *screw_norminette)
{
	while (screw_norminette->argv[*screw_norminette->i][++*screw_norminette->j]
		!= '\0')
	{
		if (!ft_strchr(screw_norminette->valid_opts,
				screw_norminette->argv[*screw_norminette->i]
				[*screw_norminette->j]))
			return (*screw_norminette->erropt
				= screw_norminette->argv[*screw_norminette->i]
				[*screw_norminette->j], *screw_norminette->optind
				= *screw_norminette->i, true);
		lpush(screw_norminette->tmp, as_getopt_arg((screw_norminette->plus_arg
					<< 8) | (int)(screw_norminette->argv
				[*screw_norminette->i][*screw_norminette->j])));
	}
	return (false);
}

/* TODO: Remove ugliness */
t_list	*ft_getopt(char *const argv[],
	char valid_opts[static 1],
	char *erropt,
	int optind[static 1])
{
	t_list	*opts;
	t_list	*tmp;
	int		i;
	int		j;

	init(&opts, &i, &erropt);
	while (argv && argv[++i])
	{
		j = 0;
		if (argv[i][j] == '-')
		{
			if (argv[i][j + 1] == '\0')
				return (*optind = i, opts);
			if (argv[i][j + 1] == '-' && argv[i][j + 2] == '\0')
				return (*optind = i + 1, opts);
			tmp = lnew();
			if (parse_arg(&(t_args){
					argv, &i, &j, optind, erropt, tmp, valid_opts, 0}))
				return (opts);
			lextend(opts, tmp);
		}
		else
			return (*optind = i, opts);
	}
	return (*optind = i, opts);
}

static t_list	*_handle_minus(t_list *opts[static 1], t_args *args)
{
	if (args->argv[*args->i][*args->j + 1] == '\0')
		return (*args->optind = *args->i, *opts);
	if (args->argv[*args->i][*args->j + 1] == '-'
		&& args->argv[*args->i][*args->j + 2] == '\0')
		return (*args->optind = *args->i + 1, *opts);
	args->tmp = lnew();
	if (parse_arg(&(t_args){
			args->argv, args->i, args->j, args->optind,
			args->erropt, args->tmp, args->valid_opts, 0}))
		return (*opts);
	lextend(*opts, args->tmp);
	return (NULL);
}

/* TODO: Remove ugliness */
t_list	*ft_getopt_plus(char *const argv[],
	char valid_opts[static 1],
	char *erropt,
	int optind[static 1])
{
	t_list	*opts;
	t_list	*tmp;
	int		i;
	int		j;

	init(&opts, &i, &erropt);
	while (argv && argv[++i])
	{
		j = 0;
		if (argv[i][j] == '-')
		{
			if (_handle_minus(&opts, &(t_args){argv, &i, &j, optind, erropt,
					tmp, valid_opts, 0}))
				return (opts);
		}
		else if (argv[i][j] == '+')
		{
			if (argv[i][j + 1] == '\0')
				return (*optind = i, opts);
			tmp = lnew();
			if (parse_arg(&(t_args){
					argv, &i, &j, optind, erropt, tmp, valid_opts, 1}))
				return (opts);
			lextend(opts, tmp);
		}
		else
			return (*optind = i, opts);
	}
	return (*optind = i, opts);
}
