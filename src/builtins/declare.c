#include "libft.h"
#include "minishell.h"

#include <unistd.h>

/* From declare_utils.c, normally static, but you know, max 5 functions... */
bool	sort_vars(t_data data1, t_data data2);
int		add_var_flags(char key[static 1],
			char *value,
			t_declare_flags flags,
			t_var *orig_var);
char	*flags_to_str(t_var *var);

#define FORCE_ANSI_C_QUOTING1 "\001\002\003\004\005\006\a\b\t\v\f\r\016\017\020"
#define FORCE_ANSI_C_QUOTING2 "\021\022\023\024\025\026\027\030\031\032\033\034"
#define FORCE_ANSI_C_QUOTING3 "\035\036\037\177"
#define FORCE_SINGLE_QUOTING " !'\"#$&()*;<>?[\\]^`{|}~"

static bool	force_ansi_c_quoting(char *s)
{
	static const char	*force_ansi_c_quoting = FORCE_ANSI_C_QUOTING1 \
											FORCE_ANSI_C_QUOTING2 \
											FORCE_ANSI_C_QUOTING3;

	while (*s)
		if (ft_strchr(force_ansi_c_quoting, *s++))
			return (true);
	return (false);
}

static bool	force_single_quoting(char *s)
{
	while (*s)
		if (ft_strchr(FORCE_SINGLE_QUOTING, *s++))
			return (true);
	return (false);
}

static char	*quote_single(char *s)
{
	char	*ret;

	ret = "'";
	while (*s)
	{
		if (*s == '\'')
			ret = ft_strjoin(ret, "'\"'\"'");
		else
			ret = ft_strjoin(ret, ft_strndup(s, 1));
		++s;
	}
	return (ft_strjoin(ret, "'"));
}
/* Runs in quadratic time... */

static char	*quote_double(char *s)
{
	char	*ret;

	ret = "\"";
	while (*s)
	{
		if (*s == '$')
			ret = ft_strjoin(ret, "\"'$'\"");
		else if (*s == '"')
			ret = ft_strjoin(ret, "\"'\"'\"");
		else
			ret = ft_strjoin(ret, ft_strndup(s, 1));
		++s;
	}
	return (ft_strjoin(ret, "\""));
}
/* Rules are adapted for minishell, not bash, so ` and \ are not escaped */
/* $ is not backslash escaped, since not required */
/* Runs in quadratic time... */

static char	*quote_ansi_c(char *s, bool bare_declare)
{
	if (bare_declare)
		return (quote_single(s));
	return (quote_double(s));
}
/* bare_declare will be removed in the future when this function is
 * actually implemented
 */

static char	*quote(char *s, bool bare_declare)
{
	if (bare_declare)
	{
		if (force_ansi_c_quoting(s))
			return (quote_ansi_c(s, bare_declare));
		else if (force_single_quoting(s))
			return (quote_single(s));
		return (s);
	}
	else
	{
		if (force_ansi_c_quoting(s))
			return (quote_ansi_c(s, bare_declare));
		return (quote_double(s));
	}
}

static void	print_var(t_kv_pair *kv, t_fds fds)
{
	if (kv->v.as_var->value)
		ft_dprintf(fds.fd_out, "declare -%s %s=%s\n", flags_to_str(kv->v.as_var), kv->k,
			quote(kv->v.as_var->value, false));
	else
		ft_dprintf(fds.fd_out, "declare -%s %s\n", flags_to_str(kv->v.as_var), kv->k);
}

bool	valid_name(char *s)
{
	if (!ft_isalpha(*s) && !(*s == '_'))
		return (false);
	++s;
	while (*s)
	{
		if (!ft_isalnum(*s) && !(*s == '_'))
			return (false);
		++s;
	}
	return (true);
}

static int	declare_print(char *name, char **argv, t_fds fds)
{
	int		exit_status;
	t_var	*var;

	exit_status = 0;
	while (*argv)
	{
		var = get_var(*argv);
		if (!valid_name(*argv))
			exit_status = minishell_error(1, false, "%s: `%s': not found", name, *argv);
		else if (var == NULL || var->special)
			exit_status = minishell_error(1, false, "%s: %s: not found", name, *argv);
		else
			print_var(&(t_kv_pair){.k = *argv, .v = as_var(var)}, fds);
		++argv;
	}
	return (exit_status);
}

static bool	has_flag(t_var *var, t_declare_flags flags)
{

	if (flags.export && var->exp)
		return (true);
	if (flags.readonly &&var->readonly)
		return (true);
	if (!(flags.export || flags.readonly))
		return (true);
	return (false);
}

static int	declare_print_all(t_declare_flags flags, t_fds fds)
{
	t_list	*vars;

	vars = liter(lsort(ht_to_list(get_vars()), sort_vars));
	while (lnext(vars))
	{
		if (has_flag(vars->current->as_kv_pair->v.as_var, flags)
			&& !vars->current->as_kv_pair->v.as_var->special)
			print_var(vars->current->as_kv_pair, fds);
	}
	return (0);
}

static int	declare_set(char *name, char **argv, t_declare_flags flags)
{
	t_list	*key_value;
	char	*key;
	char	*value;
	t_var	*orig_var;
	int		exit_status;

	exit_status = 0;
	while (*argv)
	{
		key_value = lsplit_n(*argv, "=", 1);
		key = key_value->first->as_str;
		value = key_value->first->next->as_str;
		orig_var = get_var(key);
		if (!valid_name(key))
			exit_status = minishell_error(1, false, "%s: `%s': not a valid identifier", name, key);
		else if (key_value->len == 2)
		{
			if (orig_var && orig_var->readonly)
			{
				exit_status = minishell_error(1, false,
						"%s: %s: readonly variable", name, key);
				value = orig_var->value;
			}
			exit_status = add_var_flags(key, value, flags, orig_var);
		}
		else if (key_value->len == 1 && orig_var)
			exit_status = add_var_flags(key, orig_var->value, flags, orig_var);
		else if (key_value->len == 1)
			exit_status = add_var_flags(key, NULL, flags, orig_var); // TODO: Must not change value of readonly var!
		++argv;
	}
	return (exit_status);
}

static int	declare_print_set_vars(t_fds fds)
{
	t_list	*vars;

	vars = liter(lsort(ht_to_list(get_vars()), sort_vars));
	while (lnext(vars))
	{
		if (vars->current->as_kv_pair->v.as_var->value
			&& !vars->current->as_kv_pair->v.as_var->special)
			ft_dprintf(fds.fd_out, "%s=%s\n",
				vars->current->as_kv_pair->k,
				quote(vars->current->as_kv_pair->v.as_var->value, true));
	}
	return (0);
}

int	builtin_declare(char **argv, t_fds fds)
{
	t_list			*opts;
	char			erropt;
	t_declare_flags	flags;

	opts = liter(ft_getopt_plus(argv, "pxr", &erropt, &optind));
	if (erropt)
		return (llast(opts), minishell_error(1, false,
				"%s: -%c: invalid option", argv[0], erropt));
	flags = (struct s_declare_flags){0};
	while (lnext(opts))
	{
		if ((char)opts->current->as_getopt_arg == 'p')
			flags.print = true;
		if ((char)opts->current->as_getopt_arg == 'r')
		{
			if (opts->current->as_getopt_arg & 1 << 8)
				flags.not_readonly = true;
			else
				flags.readonly = true;
		}
		if ((char)opts->current->as_getopt_arg == 'x')
		{
			if (opts->current->as_getopt_arg & 1 << 8)
				flags.not_export = true;
			else
				flags.export = true;
		}
	}
	if (flags.print && argv[optind] != NULL)
		return (declare_print(argv[0], argv + optind, fds));
	else if (flags.print || (argv[optind] == NULL && (flags.export || flags.readonly)))
		return (declare_print_all(flags, fds));
	else if (argv[optind] == NULL)
		return (declare_print_set_vars(fds));
	else
		return (declare_set(argv[0], argv + optind, flags));
}
/* Cannot use `+' for removing attributes */
