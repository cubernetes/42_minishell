/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:02:52 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:02:53 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_H
# define FT_GETOPT_H 1

# include "libft.h"

typedef struct s_args
{
	char *const	*argv;
	int			*i;
	int			*j;
	int			*optind;
	char		*erropt;
	t_list		*tmp;
	char		*valid_opts;
	bool		plus_arg;
}	t_args;

#endif /* ft_getopt.h. */
