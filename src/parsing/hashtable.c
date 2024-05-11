t_type	as_token_type(t_token_type token_type)
{
	return ((t_type){
		.t2 = token_type
	});
}

t_type	as_tree_type(t_tree_type tree_type)
{
	return ((t_type){
		.t1 = tree_type
	});
}

t_tree_type	tree_ht_get(char *key)
{
	static t_kv	*ht[TABLE_SIZE + 1];

	gc_set_null((void **)&ht[TABLE_SIZE]);
	if (ht[TABLE_SIZE] != NULL)
		return (ht_get(ht, key).t1);
	ft_bzero(ht, sizeof(*ht) * (TABLE_SIZE + 1));
	ht_set(ht, "<pipe_sequence>", as_tree_type(PIPE_SEQUENCE));
	ht_set(ht, "<complete_command_tail>", as_tree_type(COMPLETE_COMMAND_TAIL));
	ht_set(ht, "<and_or>", as_tree_type(AND_OR));
	ht_set(ht, "<command>", as_tree_type(COMMAND));
	ht_set(ht, "<pipe_sequence_tail>", as_tree_type(PIPE_SEQUENCE_TAIL));
	ht_set(ht, "<simple_command>", as_tree_type(SIMPLE_COMMAND));
	ht_set(ht, "<compound_command>", as_tree_type(COMPOUND_COMMAND));
	ht_set(ht, "<complete_command>", as_tree_type(COMPLETE_COMMAND));
	ht_set(ht, "<io_redirect>", as_tree_type(IO_REDIRECT));
	ht_set(ht, "<simple_command_tail>", as_tree_type(SIMPLE_COMMAND_TAIL));
	(ht_set(ht, "(", as_tree_type(TOKEN)), ht_set(ht, ")", as_tree_type(TOKEN)));
	(ht_set(ht, ">", as_tree_type(TOKEN)), ht_set(ht, "<", as_tree_type(TOKEN)));
	(ht_set(ht, "||", as_tree_type(TOKEN)), ht_set(ht, "&&", as_tree_type(TOKEN)));
	ht_set(ht, ">>", as_tree_type(TOKEN));
	ht_set(ht, "<<", as_tree_type(TOKEN));
	ht_set(ht, "TOK_WORD", as_tree_type(TOKEN));
	ht_set(ht, "TOK_EPSILON", as_tree_type(TOKEN));
	ht_set(ht, "|", as_tree_type(TOKEN));
	ht[TABLE_SIZE] = (void *)1;
	return (ht_get(ht, key).t1);
}

t_token_type	tokens_ht_get(char *key)
{
	static t_kv	*ht[TABLE_SIZE + 1];

	gc_set_null((void **)&ht[TABLE_SIZE]);
	if (ht[TABLE_SIZE] == NULL)
	{
		ft_bzero(ht, sizeof(*ht) * (TABLE_SIZE + 1));
		ht_set(ht, "TOK_EPSILON", as_token_type(TOK_EPSILON));
		ht_set(ht, "TOK_WORD", as_token_type(TOK_WORD));
		ht_set(ht, "<<", as_token_type(TOK_HEREDOC));
		ht_set(ht, ">>", as_token_type(TOK_APPEND));
		ht_set(ht, "<", as_token_type(TOK_INPUT));
		ht_set(ht, ">", as_token_type(TOK_OVERRIDE));
		ht_set(ht, "(", as_token_type(TOK_L_PAREN));
		ht_set(ht, ")", as_token_type(TOK_R_PAREN));
		ht_set(ht, "&&", as_token_type(TOK_AND));
		ht_set(ht, "||", as_token_type(TOK_OR));
		ht_set(ht, "|", as_token_type(TOK_PIPE));
		ht[TABLE_SIZE] = (void *)1;
	}
	return (ht_get(ht, key).t2);
}
