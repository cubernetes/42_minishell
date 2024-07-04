/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 20:08:09 by tosuman           #+#    #+#             */
/*   Updated: 2024/07/05 00:52:29 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_getopt.h"

#include <stdlib.h>

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

static t_list	*_handle_minus(t_list *opts[static 1], t_args *args)
{
	t_list	*tmp;

	if (args->argv[*args->i][*args->j + 1] == '\0')
		return (*args->optind = *args->i, *opts);
	if (args->argv[*args->i][*args->j + 1] == '-'
		&& args->argv[*args->i][*args->j + 2] == '\0')
		return (*args->optind = *args->i + 1, *opts);
	tmp = lnew();
	if (parse_arg(&(t_args){
			args->argv, args->i, args->j, args->optind,
			args->erropt, tmp, args->valid_opts, false}))
		return (*opts);
	lextend(*opts, tmp);
	return (NULL);
}

static t_list	*_handle_plus(t_list *opts[static 1], t_args *args)
{
	t_list	*tmp;

	if (args->argv[*args->i][*args->j + 1] == '\0')
		return (*args->optind = *args->i, *opts);
	tmp = lnew();
	if (parse_arg(&(t_args){
			args->argv, args->i, args->j, args->optind,
			args->erropt, tmp, args->valid_opts, true}))
		return (*opts);
	lextend(*opts, tmp);
	return (NULL);
}

t_list	*ft_getopt(char *const argv[],
	char valid_opts[static 1],
	char *erropt,
	int optind[static 1])
{
	t_list	*opts;
	int		i;
	int		j;

	if (erropt == NULL)
		erropt = ft_malloc(sizeof(char));
	opts = lnew();
	i = 0;
	*erropt = '\0';
	while (argv && argv[++i])
	{
		j = 0;
		if (argv[i][j] != '-')
			return (*optind = i, opts);
		else if (_handle_minus(&opts, &(t_args){argv,
				&i, &j, optind, erropt, NULL, valid_opts, false}))
			return (opts);
	}
	return (*optind = i, opts);
}

t_list	*ft_getopt_plus(char *const argv[],
	char valid_opts[static 1],
	char *erropt,
	int optind[static 1])
{
	t_list			*opts;
	int				i;
	int				j;
	static t_list	*(*funcs[2])(t_list **opts, t_args *args)
		= {_handle_minus, _handle_plus};

	if (erropt == NULL)
		erropt = ft_malloc(sizeof(char));
	opts = lnew();
	i = 0;
	*erropt = '\0';
	while (argv && argv[++i])
	{
		j = 0;
		if (!ft_strchr("-+", argv[i][j]))
			return (*optind = i, opts);
		else if (funcs[argv[i][j] == '+'](&opts, &(t_args){argv,
				&i, &j, optind, erropt, NULL, valid_opts, false}))
			return (opts);
	}
	return (*optind = i, opts);
}
