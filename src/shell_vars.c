#include "minishell.h"
#include "libft.h"

/** Set a shell variable or get it, depending on the parameters.
 *
 *  if `key' is not NULL:
 *   @param key: the variable name
 *   @param value: the variable value
 *   @param exp: whether this variable should be inherited by child processes
 *   @returns: `value'
 *  if `key' is NULL:
 *   @param value: the variable name
 *   @returns: the variable value corresponding to `value';
 *             "" if the variable is unset
 */
char	*set_var(char *key, char value[static 1], t_flags flags)
{
	static t_ht	*shell_vars[MAX_HT_SIZE];
	char		*ret;

	if (key == NULL)
	{
		ret = ht_get(shell_vars, value).as_var->value;
		if (ret == NULL)
			return ("");
		return (ret);
	}
	gc_set_context("POST");
	ht_set(shell_vars, key,
		(t_data){.as_var = ft_memdup(&(t_var){
			.exp = flags.exp,
			.readonly = flags.readonly,
			.hidden = flags.hidden,
			.value = ft_strdup(value)
		}, sizeof(t_var))});
	gc_set_context("DEFAULT");
	return (value);
}

/** Wrapper the get a shell variable.
 *  
 *  @param key: the variable name
 *  @returns: the variable value corresponding to `key';
 *            "" if the variable is unset
 */
char	*get_var(char key[static 1])
{
	return (set_var(NULL, key, (t_flags){0}));
}
