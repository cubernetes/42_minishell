#include "minishell.h"
#include "libft.h"

/** Set a shell variable or get it, or get all, depending on the parameters.
 *
 *  if `key_or_ret' is not NULL:
 *   @param key_or_ret: the variable name
 *   @param value_or_key: the variable value_or_key (NULL means unset)
 *   @param flags: the flags the variable shall have (readonly, export, etc.)
 *   @returns: NULL
 *  if `key_or_ret' is NULL:
 *	  if `value_or_key' is NULL:
 *	    @returns: the shell variables hashtable
 *	  if `value_or_key' is not NULL:
 *      @param value_or_key: the variable name
 *      @returns via key_or_ret: the value relating to the key `value_or_key';
 *                               "" if the variable is unset
 *      @returns: NULL
 */
static t_ht	**var_manager(char *key_or_ret[static 1], char *value_or_key,
	t_flags flags)
{
	static t_ht	*shell_vars[MAX_HT_SIZE];
	t_var		*ret;

	if (*key_or_ret == NULL)
	{
		if (value_or_key == NULL)
			return (shell_vars);
		ret = ht_get(shell_vars, value_or_key).as_var;
		*key_or_ret = (char *)ret;
		return (NULL);
	}
	if (flags.unset)
		return (ret = ht_get(shell_vars, *key_or_ret).as_var,
			ht_unset(shell_vars, *key_or_ret),
			*key_or_ret = ret->value, NULL);
	gc_start_context("POST");
	ht_set(shell_vars, *key_or_ret,
		as_var(ft_memdup(&(t_var){
				.exp = flags.exp,
				.readonly = flags.readonly,
				.special = flags.special,
				.value = ft_nullable_strdup(value_or_key)
			}, sizeof(t_var))));
	gc_end_context();
	return (NULL);
}

/** Wrapper to get a shell variable.
 *  
 *  @param key: the variable name
 *  @returns: the t_var* corresponding to `key'
 */
t_var	*get_var(char key[static 1])
{
	t_var	*ret;

	ret = NULL;
	var_manager((char **)&ret, key, (t_flags){0});
	return (ret);
}

bool	unset_var(char key[static 1])
{
	t_ht	**vars;

	vars = get_vars();
	if (ht_get(vars, key).as_var)
	{
		if (ht_get(vars, key).as_var->readonly)
			return (false);
		var_manager(&key, NULL, (t_flags){.unset = true});
	}
	return (true);
}

void	set_var(char key[static 1], char *value, t_flags flags)
{
	var_manager(&key, value, flags);
}

t_ht	**get_vars(void)
{
	t_ht	**vars;

	vars = var_manager(&(char *){0}, NULL, (t_flags){0});
	return (vars);
}
