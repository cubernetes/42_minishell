#include "minishell.h"
#include "libft.h"

/* TODO: Not required: hashtable */
const char	*tree_type_to_string(t_tree_type type)
{
	if (type == COMPLETE_COMMAND)
		return (STR_COMPLETE_COMMAND);
	else if (type == COMPLETE_COMMAND_TAIL)
		return (STR_COMPLETE_COMMAND_TAIL);
	else if (type == AND_OR)
		return (STR_AND_OR);
	else if (type == PIPE_SEQUENCE)
		return (STR_PIPE_SEQUENCE);
	else if (type == PIPE_SEQUENCE_TAIL)
		return (STR_PIPE_SEQUENCE_TAIL);
	else if (type == COMMAND)
		return (STR_COMMAND);
	else if (type == COMPOUND_COMMAND)
		return (STR_COMPOUND_COMMAND);
	else if (type == SIMPLE_COMMAND)
		return (STR_SIMPLE_COMMAND);
	else if (type == SIMPLE_COMMAND_TAIL)
		return (STR_SIMPLE_COMMAND_TAIL);
	else if (type == IO_REDIRECT)
		return (STR_IO_REDIRECT);
	else if (type == TOKEN)
		return (STR_TOKEN);
	else
		return (STR_TREE_TYPE_UNKNOWN);
}

bool	tree_is_null(t_tree *tree)
{
	if (!tree)
		return (true);
	if (!ft_memcmp(tree, &(t_tree){0}, sizeof(t_tree)))
		return (true);
	return (false);
}

t_list	*creat_new_children(t_tree **children)
{
	t_list	*new_children;
	size_t	i;

	i = 0;
	new_children = lnew();
	while (children[i])
		lpush(new_children, as_tree(children[i++]));
	return (new_children);
}

/* return the index of the production to use */
/* TODO: fix error handling */
/* TODO: Not required: Make transition table adaptive (quite a lot of work) */
int	get_production_idx(t_tree_type nonterm, t_token *token)
{
	static int		transition_table[NUM_NONTERMS][NUM_TOKENS] = {
	{19, -1, -1, -1, 0, -1, 0, 0, 0, 0, 0},
	{1, 2, 2, -1, -1, 1, -1, -1, -1, -1, -1},
	{-1, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, 5, -1, 5, 5, 5, 5, 5},
	{6, 6, 6, 7, -1, 6, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, 9, -1, 8, 8, 8, 8, 8},
	{-1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, 11, 11, 11, 11, 11},
	{12, 12, 12, 12, -1, 12, 13, 13, 13, 13, 13},
	{-1, -1, -1, -1, -1, -1, 14, 17, 15, 18, 16}
	};
	int				production_idx;
	t_token_type	type;

	type = token->type;
	if (type == TOK_DQUOTE_STR || type == TOK_SQUOTE_STR)
		type = TOK_WORD;
	else if (type == TOK_SEMI)
		type = TOK_AND;
	else if (type == TOK_OVERRIDE_ERR)
		type = TOK_OVERRIDE;
	else if (type == TOK_APPEND_ERR)
		type = TOK_APPEND;
	production_idx = transition_table[nonterm - 1][type - 1];
	if (production_idx == -1)
	{
		if (!*token->str)
			set_last_exit_status(minishell_error(2, !option_enabled('i'), true,
				"syntax error near unexpected token `newline'"));
		else
			set_last_exit_status(minishell_error(2, !option_enabled('i'), true, "syntax error near unexpected token `%s'",
				token->str));
	}
	return (production_idx);
}
