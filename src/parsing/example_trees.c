#include "minishell.h"

t_tree	*return_example_parse_tree(void)
{
	return (\
	new_tree_nonterm(COMPLETE_COMMAND, creat_new_children((t_children){\
		new_tree_nonterm(PIPE_SEQUENCE, creat_new_children((t_children){\
			new_tree_nonterm(COMMAND, creat_new_children((t_children){\
				new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
					new_tree_m_token(TOK_WORD, "echo"), \
					new_tree_m_token(TOK_WORD, "hi"), \
				NULL})), \
			NULL})), \
			new_tree_m_token(TOK_PIPE, "|"), \
			new_tree_nonterm(COMMAND, creat_new_children((t_children){\
				new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
					new_tree_m_token(TOK_WORD, "grep"), \
					new_tree_m_token(TOK_WORD, "^h"), \
				NULL})), \
			NULL})),
		NULL})),
		new_tree_m_token(TOK_AND, "&&"),
		new_tree_nonterm(PIPE_SEQUENCE, creat_new_children((t_children){
			new_tree_nonterm(COMMAND, creat_new_children((t_children){
				new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){
					new_tree_m_token(TOK_WORD, "echo"),
					new_tree_m_token(TOK_SQUOTE_STR, "hello world"),
				NULL})),
			NULL})),
			new_tree_m_token(TOK_PIPE, "|"),
			new_tree_nonterm(COMMAND, creat_new_children((t_children){
				new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){
					new_tree_m_token(TOK_WORD, "grep"),
					new_tree_m_token(TOK_DQUOTE_STR, "^hello world"),
				NULL})),
			NULL})),
		NULL})),
	NULL}))
	);
}

t_tree	*return_example_ast(void)
{
	return (\
	new_tree_nonterm(COMPLETE_COMMAND, creat_new_children((t_children){\
		new_tree_nonterm(PIPE_SEQUENCE, creat_new_children((t_children){\
			new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
				new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
					new_tree_m_token(TOK_HEREDOC, "<<"), \
					new_tree_m_token(TOK_WORD, "heredoc"), \
				NULL})), \
				new_tree_m_token(TOK_WORD, "echo"), \
				new_tree_m_token(TOK_WORD, "hi"), \
			NULL})), \
			new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
				new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
					new_tree_m_token(TOK_INPUT, "<"), \
					new_tree_m_token(TOK_WORD, "infile"), \
				NULL})), \
				new_tree_m_token(TOK_WORD, "grep"), \
				new_tree_m_token(TOK_WORD, "^h"), \
				new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
					new_tree_m_token(TOK_OVERRIDE, ">"), \
					new_tree_m_token(TOK_WORD, "somefile"), \
				NULL})), \
				new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
					new_tree_m_token(TOK_OVERRIDE, ">"), \
					new_tree_m_token(TOK_WORD, "anotherfile"), \
				NULL})), \
			NULL})), \
			new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
				new_tree_m_token(TOK_WORD, "nl"), \
			NULL})), \
		NULL})), \
		new_tree_m_token(TOK_AND, "&&"), \
		new_tree_nonterm(PIPE_SEQUENCE, creat_new_children((t_children){\
			new_tree_nonterm(COMPLETE_COMMAND, creat_new_children((t_children){\
				new_tree_nonterm(PIPE_SEQUENCE, creat_new_children((t_children){\
					new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
						new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
							new_tree_m_token(TOK_HEREDOC, "<<"), \
							new_tree_m_token(TOK_WORD, "heredoc"), \
						NULL})), \
						new_tree_m_token(TOK_WORD, "echo"), \
						new_tree_m_token(TOK_WORD, "hi"), \
					NULL})), \
					new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
						new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
							new_tree_m_token(TOK_INPUT, "<"), \
							new_tree_m_token(TOK_WORD, "infile"), \
						NULL})), \
						new_tree_m_token(TOK_WORD, "grep"), \
						new_tree_m_token(TOK_WORD, "^h"), \
						new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
							new_tree_m_token(TOK_OVERRIDE, ">"), \
							new_tree_m_token(TOK_WORD, "somefile"), \
						NULL})), \
						new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
							new_tree_m_token(TOK_OVERRIDE_ERR, "2>"), \
							new_tree_m_token(TOK_WORD, "anotherfile"), \
						NULL})), \
					NULL})), \
					new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
						new_tree_m_token(TOK_WORD, "nl"), \
					NULL})), \
				NULL})), \
			NULL})), \
			new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
				new_tree_m_token(TOK_WORD, "grep"), \
				new_tree_m_token(TOK_WORD, "h"), \
				new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
					new_tree_m_token(TOK_APPEND_ERR, "2>>"), \
					new_tree_m_token(TOK_WORD, "appendfile.txt"), \
				NULL})), \
			NULL})), \
		NULL})), \
		new_tree_m_token(TOK_AND, "&&"), \
		new_tree_nonterm(PIPE_SEQUENCE, creat_new_children((t_children){\
			new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
				new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
					new_tree_m_token(TOK_INPUT, "<"), \
					new_tree_m_token(TOK_WORD, "infile"), \
				NULL})), \
				new_tree_m_token(TOK_WORD, "hello world"), \
				new_tree_nonterm(IO_REDIRECT, creat_new_children((t_children){\
					new_tree_m_token(TOK_OVERRIDE, ">"), \
					new_tree_m_token(TOK_WORD, "/dev/null"), \
				NULL})), \
			NULL})), \
			new_tree_nonterm(SIMPLE_COMMAND, creat_new_children((t_children){\
				new_tree_m_token(TOK_WORD, "grep"), \
				new_tree_m_token(TOK_DQUOTE_STR, "^hello world"), \
			NULL})), \
		NULL})), \
	NULL}))
	);
}
