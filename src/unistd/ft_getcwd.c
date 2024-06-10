#include "minishell.h"
#include "libft.h"

#include <unistd.h>

/** Return the current working directory, replacing the home directory
 *  (determined from the HOME environment variable) with a tilde ("~").
 *
 *  @returns: the current working directory where the home directory (determined from
 *            the HOME environment variable) is replaced with a tilde
 */
char	*ft_getcwd(void)
{
	char	*cwd;
	char	*home_dir;

	cwd = gc_add_str(getcwd(NULL, 0));
	home_dir = var_lookup("HOME");
	if (*home_dir && !ft_strncmp(home_dir, cwd, ft_strlen(home_dir)))
		cwd = ft_replace(cwd, home_dir, "~");
	return (cwd);
}
