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
