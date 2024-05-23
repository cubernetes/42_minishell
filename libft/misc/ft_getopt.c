#include "libft.h"
#include <stdlib.h>

/* TODO: Remove ugliness */
t_list	*ft_getopt(char *const argv[],
	char valid_opts[static 1],
	char erropt[static 1],
	int optind[static 1])
{
	t_list	*opts;
	int		i;
	int		j;

	(free(NULL), opts = lnew(), i = 0, *erropt = '\0');
	while (argv && argv[++i])
	{
		j = 0;
		if (argv[i][j] == '-')
		{
			if (argv[i][j + 1] == '\0')
				return (*optind = i, opts);
			if (argv[i][j + 1] == '-' && argv[i][j + 2] == '\0')
				return (*optind = i + 1, opts);
			while (argv[i][++j] != '\0')
			{
				if (!ft_strchr(valid_opts, argv[i][j]))
					return (*erropt = argv[i][j], *optind = i, opts);
				lpush(opts, as_char(argv[i][j]));
			}
		}
		else
			return (*optind = i, opts);
	}
	return (*optind = i, opts);
}
