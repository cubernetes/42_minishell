#include "libft.h"
#include "minishell.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

char	*normalize(char *path)
{
	t_list	*parts;
	t_list	*new_parts;

	parts = liter(lsplit(path, "/"));
	new_parts = lnew();
	while (lnext(parts))
	{
		if (!ft_strcmp(parts->current->as_str, ""));
		else if (!ft_strcmp(parts->current->as_str, "."));
		else if (!ft_strcmp(parts->current->as_str, ".."))
			lpop(new_parts);
		else
			lpush(new_parts, as_data(parts->current));
	}
	return (ft_strjoin("/", ljoin(new_parts, "/")));
}

char	*set_saved_cwd(char *cwd)
{
	static char	*_cwd;

	if (cwd != NULL)
	{
		gc_start_context("POST");
		_cwd = ft_strdup(cwd);
		gc_end_context();
	}
	return (_cwd);
}

char	*get_saved_cwd(void)
{
	return (set_saved_cwd(NULL));
}
/* TODO: cd with null argument is same as cd . */
