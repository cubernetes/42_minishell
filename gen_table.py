#!/usr/bin/env python3

import re

# https://www.cs.princeton.edu/courses/archive/spring20/cos320/LL1/
# Literally copy from the website and paste inside of the triple single quote
# Note, table is still without the rule complete_command ::= ''
table = 'Nonterminal	' + '''

	$	&&	||	|	(	)	TOK_WORD	>>	<<	>	<
S					S ::= complete_command $		S ::= complete_command $	S ::= complete_command $	S ::= complete_command $	S ::= complete_command $	S ::= complete_command $
complete_command					complete_command ::= pipe_sequence complete_command_tail		complete_command ::= pipe_sequence complete_command_tail	complete_command ::= pipe_sequence complete_command_tail	complete_command ::= pipe_sequence complete_command_tail	complete_command ::= pipe_sequence complete_command_tail	complete_command ::= pipe_sequence complete_command_tail
complete_command_tail	complete_command_tail ::= ε	complete_command_tail ::= and_or pipe_sequence complete_command_tail	complete_command_tail ::= and_or pipe_sequence complete_command_tail			complete_command_tail ::= ε					
and_or		and_or ::= &&	and_or ::= ||								
pipe_sequence					pipe_sequence ::= command pipe_sequence_tail		pipe_sequence ::= command pipe_sequence_tail	pipe_sequence ::= command pipe_sequence_tail	pipe_sequence ::= command pipe_sequence_tail	pipe_sequence ::= command pipe_sequence_tail	pipe_sequence ::= command pipe_sequence_tail
pipe_sequence_tail	pipe_sequence_tail ::= ε	pipe_sequence_tail ::= ε	pipe_sequence_tail ::= ε	pipe_sequence_tail ::= | command pipe_sequence_tail		pipe_sequence_tail ::= ε					
command					command ::= compound_command		command ::= simple_command	command ::= simple_command	command ::= simple_command	command ::= simple_command	command ::= simple_command
compound_command					compound_command ::= ( complete_command )						
simple_command							simple_command ::= io_redirect TOK_WORD simple_command_tail	simple_command ::= io_redirect TOK_WORD simple_command_tail	simple_command ::= io_redirect TOK_WORD simple_command_tail	simple_command ::= io_redirect TOK_WORD simple_command_tail	simple_command ::= io_redirect TOK_WORD simple_command_tail
simple_command_tail	simple_command_tail ::= ε	simple_command_tail ::= ε	simple_command_tail ::= ε	simple_command_tail ::= ε		simple_command_tail ::= ε	simple_command_tail ::= io_redirect TOK_WORD simple_command_tail	simple_command_tail ::= io_redirect TOK_WORD simple_command_tail	simple_command_tail ::= io_redirect TOK_WORD simple_command_tail	simple_command_tail ::= io_redirect TOK_WORD simple_command_tail	simple_command_tail ::= io_redirect TOK_WORD simple_command_tail
io_redirect							io_redirect ::= ε	io_redirect ::= >>	io_redirect ::= <<	io_redirect ::= >	io_redirect ::= <

'''.strip().replace('ε', "''")
table = table.splitlines()
# Remove start symbol line
table = '\n'.join([table[0]] + table[2:]) + '\n'

# Same here
# Note for future, below grammar should add the following rule
# complete_command ::= ''
productions = '''
complete_command ::= pipe_sequence complete_command_tail
complete_command_tail ::= ''
complete_command_tail ::= and_or pipe_sequence complete_command_tail
and_or ::= &&
and_or ::= ||
pipe_sequence ::= command pipe_sequence_tail
pipe_sequence_tail ::= ''
pipe_sequence_tail ::= | command pipe_sequence_tail
command ::= simple_command
command ::= compound_command
compound_command ::= ( complete_command )
simple_command ::= io_redirect TOK_WORD simple_command_tail
simple_command_tail ::= ''
simple_command_tail ::= io_redirect TOK_WORD simple_command_tail
io_redirect ::= ''
io_redirect ::= >>
io_redirect ::= <<
io_redirect ::= >
io_redirect ::= <
'''.strip().splitlines()

# Add -1 to empty fields
for i, prod in enumerate(productions):
    table = table.replace(prod, str(i))
table = re.sub(r'\t', r';', table)
table = re.sub(r'\n', r';\n', table)
table = re.sub(r';(?=;)', r';-1', table)

print(table)
