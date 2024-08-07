# Simplified posix shell grammar for the minishell project
# Start symbol is complete_command

# Raw yacc BNF taken from:
# https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10_02
# Productions not relevant for minishell are removed.
```c yacc
%token TOK_WORD
/* %token TOK_SQUOTE_STR TOK_DQUOTE_STR */

%token TOK_AND TOK_OR TOK_PIPE TOK_L_PAREN TOK_R_PAREN
/*     '&&'    '||'   '|'      '('         ')'      */

%token TOK_OVERRIDE TOK_APPEND TOK_INPUT TOK_HEREDOC
/*     '>'          '>>'       '<'       '<<'     */

// Note, complete_command can be empty, this is missing here but added later down
complete_command : list
                 ;
list             : and_or
                 ;
and_or           : pipeline
                 | pipeline TOK_AND and_or
                 | pipeline TOK_OR and_or
                 ;
pipeline         : pipe_sequence
                 ;
pipe_sequence    : command
                 | command TOK_PIPE pipe_sequence
                 ;
command          : simple_command
                 | compound_command
                 | compound_command redirect_list
                 ;
compound_command : subshell
                 ;
subshell         : TOK_L_PAREN compound_list TOK_R_PAREN
                 ;
compound_list    : term
                 ;
term             : and_or
                 ;
simple_command   : cmd_prefix cmd_word cmd_suffix
                 | cmd_prefix cmd_word
                 | cmd_prefix
                 | cmd_name cmd_suffix
                 | cmd_name
                 ;
cmd_name         : TOK_WORD
                 ;
cmd_word         : TOK_WORD
                 ;
cmd_prefix       : io_redirect
                 | io_redirect cmd_prefix
                 |            TOK_WORD
                 | cmd_prefix TOK_WORD
                 ;
cmd_suffix       : io_redirect
                 | io_redirect cmd_prefix
                 |            TOK_WORD
                 | cmd_suffix TOK_WORD
                 ;
redirect_list    : io_redirect
                 | io_redirect redirect_list
                 ;
io_redirect      : io_file
                 | io_here
                 ;
io_file          : TOK_APPEND   filename
                 | TOK_OVERRIDE filename
                 | TOK_INPUT    filename
                 ;
filename         : TOK_WORD
                 ;
io_here          : TOK_HEREDOC here_end
                 ;
here_end         : WORD
                 ;
```

# Same grammar as but this time in EBNF
# Note, complete_command can be empty, this is missing here but added later down
```c
complete_command = list ;
list = and_or ;
and_or = pipeline { ( TOK_AND | TOK_OR ) pipeline } ;
pipeline = pipe_sequence ;
pipe_sequence = command { TOK_PIPE command } ;
command = simple_command | compound_command [ redirect_list ] ;
compound_command = subshell ;
subshell = TOK_L_PAREN compound_list TOK_R_PAREN ;
compound_list = term ;
term = and_or ;
simple_command = cmd_prefix | [ cmd_prefix ] cmd_word [ cmd_suffix ] ;
cmd_word = TOK_WORD ;
cmd_prefix = ( cmd_word | io_redirect ) { cmd_word | io_redirect } ;
cmd_suffix = ( cmd_word | io_redirect ) { cmd_word | io_redirect } ;
io_redirect = io_file | io_here ;
io_file = ( TOK_APPEND | TOK_OVERRIDE | TOK_INPUT ) filename ;
filename = TOK_WORD ;
io_here = TOK_HEREDOC here_end ;
here_end = TOK_WORD ;
```

# Compressed form
# Note, complete_command can be empty, this is missing here but added later down
```c
complete_command = pipe_sequence { ( TOK_AND | TOK_OR ) pipe_sequence } ;
pipe_sequence = command { TOK_PIPE command } ;
command = simple_command | compound_command { io_redirect } ;
compound_command = TOK_L_PAREN complete_command TOK_R_PAREN ; # aka subshell
simple_command = ( TOK_WORD | io_redirect ) { TOK_WORD | io_redirect } ;
io_redirect = ( TOK_APPEND | TOK_OVERRIDE | TOK_INPUT | TOK_HEREDOC ) TOK_WORD ;
```

# Improved form. (shockingly similar to lmiehler's EBNF)
# Note, complete_command can be empty, this is missing here but added later down
```c
complete_command = pipe_sequence { ( '&&' | '||' ) pipe_sequence } ;
pipe_sequence = command { '|' command } ;
command = simple_command | compound_command ;
compound_command = '(' complete_command ')' ; # aka subshell
simple_command = [ io_redirect ] TOK_WORD { [ io_redirect ] TOK_WORD } ;
io_redirect = '>' | '>>' | '<' | '<<' ;
```
# Remarks:
# - TOK_WORD will never be an ASSIGMENT_WORD. ASSIGNMENT_WORD's are only used
# as the arguments on the `export' builtin, but that 'simple' parsing is done
# separately
# - Since TOK_WORD is never an ASSIGMENT_WORD, it'll be either a builtin or
# a program name, nothing else! Therefore, TOK_WORD can consist of any
# character except NUL and the slash '/', all after wildcard expansion,
# environment variable expansion, and quote handling.


# Converted to LL(1)-grammar in verbose BNF
# verified LL(1) with https://www.cs.princeton.edu/courses/archive/spring20/cos320/LL1/
# '' represents empty string (epsilon)
# whitespace delimited tokens found on the right, which are not found on the left
# are terminal symbols. Every other symbol, except '', is a non-terminal.
# Update, complete_command can be empty
```c
complete_command ::= ''
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
```

# This yields the following first & follow sets:
# Update, make complete_command nullable
```c
Nonterminal           | Nullable? | First set                 | Follow set
----------------------|-----------|---------------------------|-----------
complete_command      | yes       | TOK_WORD, (, >, >>, <, << | ), $
complete_command_tail | yes       | &&, ||                    | ), $
and_or                | no        | &&, ||                    | TOK_WORD, (, >, >>, <, <<
pipe_sequence         | no        | TOK_WORD, (, >, >>, <, << | &&, ||, ), $
pipe_sequence_tail    | yes       | |                         | &&, ||, ), $
command               | no        | TOK_WORD, (, >, >>, <, << | &&, ||, |, ), $
compound_command      | no        | (                         | &&, ||, |, ), $
simple_command        | no        | TOK_WORD, >, >>, <, <<    | &&, ||, |, ), $
simple_command_tail   | yes       | TOK_WORD, >, >>, <, <<    | &&, ||, |, ), $
io_redirect           | yes       | >, >>, <, <<              | TOK_WORD
```

# And it yields the following transition table (generated from the link above)
# Update, add rule 19 (complete_command ::= '')
```c
Nonterminal           | $                            | &&                                                                   | ||                                                                   | |                                                   | (                                                        | )                            | TOK_WORD                                                         | >                                                                | >>                                                               | <                                                                | <<
----------------------|------------------------------|----------------------------------------------------------------------|----------------------------------------------------------------------|-----------------------------------------------------|----------------------------------------------------------|------------------------------|------------------------------------------------------------------|------------------------------------------------------------------|------------------------------------------------------------------|------------------------------------------------------------------|-----------------------------------------------------------------
complete_command      | complete_command ::= ''      |                                                                      |                                                                      |                                                     | complete_command ::= pipe_sequence complete_command_tail |                              | complete_command ::= pipe_sequence complete_command_tail         | complete_command ::= pipe_sequence complete_command_tail         | complete_command ::= pipe_sequence complete_command_tail         | complete_command ::= pipe_sequence complete_command_tail         | complete_command ::= pipe_sequence complete_command_tail
complete_command_tail | complete_command_tail ::= '' | complete_command_tail ::= and_or pipe_sequence complete_command_tail | complete_command_tail ::= and_or pipe_sequence complete_command_tail |                                                     |                                                          | complete_command_tail ::= '' |                                                                  |                                                                  |                                                                  |                                                                  |
and_or                |                              | and_or ::= &&                                                        | and_or ::= ||                                                        |                                                     |                                                          |                              |                                                                  |                                                                  |                                                                  |                                                                  |
pipe_sequence         |                              |                                                                      |                                                                      |                                                     | pipe_sequence ::= command pipe_sequence_tail             |                              | pipe_sequence ::= command pipe_sequence_tail                     | pipe_sequence ::= command pipe_sequence_tail                     | pipe_sequence ::= command pipe_sequence_tail                     | pipe_sequence ::= command pipe_sequence_tail                     | pipe_sequence ::= command pipe_sequence_tail
pipe_sequence_tail    | pipe_sequence_tail ::= ''    | pipe_sequence_tail ::= ''                                            | pipe_sequence_tail ::= ''                                            | pipe_sequence_tail ::= | command pipe_sequence_tail |                                                          | pipe_sequence_tail ::= ''    |                                                                  |                                                                  |                                                                  |                                                                  |
command               |                              |                                                                      |                                                                      |                                                     | command ::= compound_command                             |                              | command ::= simple_command                                       | command ::= simple_command                                       | command ::= simple_command                                       | command ::= simple_command                                       | command ::= simple_command
compound_command      |                              |                                                                      |                                                                      |                                                     | compound_command ::= ( complete_command )                |                              |                                                                  |                                                                  |                                                                  |                                                                  |
simple_command        |                              |                                                                      |                                                                      |                                                     |                                                          |                              | simple_command ::= io_redirect TOK_WORD simple_command_tail      | simple_command ::= io_redirect TOK_WORD simple_command_tail      | simple_command ::= io_redirect TOK_WORD simple_command_tail      | simple_command ::= io_redirect TOK_WORD simple_command_tail      | simple_command ::= io_redirect TOK_WORD simple_command_tail
simple_command_tail   | simple_command_tail ::= ''   | simple_command_tail ::= ''                                           | simple_command_tail ::= ''                                           | simple_command_tail ::= ''                          |                                                          | simple_command_tail ::= ''   | simple_command_tail ::= io_redirect TOK_WORD simple_command_tail | simple_command_tail ::= io_redirect TOK_WORD simple_command_tail | simple_command_tail ::= io_redirect TOK_WORD simple_command_tail | simple_command_tail ::= io_redirect TOK_WORD simple_command_tail | simple_command_tail ::= io_redirect TOK_WORD simple_command_tail
io_redirect           |                              |                                                                      |                                                                      |                                                     |                                                          |                              | io_redirect ::= ''                                               | io_redirect ::= >                                                | io_redirect ::= >>                                               | io_redirect ::= <                                                | io_redirect ::= <<
```

# Let's index the production rules
```c
 0. complete_command ::= pipe_sequence complete_command_tail
 1. complete_command_tail ::= ''
 2. complete_command_tail ::= and_or pipe_sequence complete_command_tail
 3. and_or ::= &&
 4. and_or ::= ||
 5. pipe_sequence ::= command pipe_sequence_tail
 6. pipe_sequence_tail ::= ''
 7. pipe_sequence_tail ::= | command pipe_sequence_tail
 8. command ::= simple_command
 9. command ::= compound_command
10. compound_command ::= ( complete_command )
11. simple_command ::= io_redirect TOK_WORD simple_command_tail
12. simple_command_tail ::= ''
13. simple_command_tail ::= io_redirect TOK_WORD simple_command_tail
14. io_redirect ::= ''
15. io_redirect ::= >>
16. io_redirect ::= <<
17. io_redirect ::= >
18. io_redirect ::= <
```

# Addendum, rule 19, complete_command may be empty
```c
 19. complete_command ::= ''
```


# This yields the same transition table, this time with the rule index instead
# of the production itself
# Update, add rule 19
```c
Nonterminal           | $  | && | || | |  | (  | )  | TOK_WORD | >> | << | >  | <
----------------------|----|----|----|----|----|----|----------|----|----|----|--
complete_command      | 19 |    |    |    | 0  |    | 0        | 0  | 0  | 0  | 0
complete_command_tail | 1  | 2  | 2  |    |    | 1  |          |    |    |    |
and_or                |    | 3  | 4  |    |    |    |          |    |    |    |
pipe_sequence         |    |    |    |    | 5  |    | 5        | 5  | 5  | 5  | 5
pipe_sequence_tail    | 6  | 6  | 6  | 7  |    | 6  |          |    |    |    |
command               |    |    |    |    | 9  |    | 8        | 8  | 8  | 8  | 8
compound_command      |    |    |    |    | 10 |    |          |    |    |    |
simple_command        |    |    |    |    |    |    | 11       | 11 | 11 | 11 | 1
simple_command_tail   | 12 | 12 | 12 | 12 |    | 12 | 13       | 13 | 13 | 13 | 1
io_redirect           |    |    |    |    |    |    | 14       | 15 | 16 | 17 | 1
```

# Let's place in the original enum identifiers
# Update, add rule 19
```c
Nonterminal           | TOK_EOL | TOK_AND | TOK_OR | TOK_PIPE | TOK_L_PAREN | TOK_R_PAREN | TOK_WORD | TOK_APPEND | TOK_HEREDOC | TOK_OVERRIDE | TOK_INPUT
----------------------|---------|---------|--------|----------|-------------|-------------|----------|------------|-------------|--------------|----------
COMPLETE_COMMAND      | 19      |         |        |          | 0           |             | 0        | 0          | 0           | 0            | 0
COMPLETE_COMMAND_TAIL | 1       | 2       | 2      |          |             | 1           |          |            |             |              |
AND_OR                |         | 3       | 4      |          |             |             |          |            |             |              |
PIPE_SEQUENCE         |         |         |        |          | 5           |             | 5        | 5          | 5           | 5            | 5
PIPE_SEQUENCE_TAIL    | 6       | 6       | 6      | 7        |             | 6           |          |            |             |              |
COMMAND               |         |         |        |          | 9           |             | 8        | 8          | 8           | 8            | 8
COMPOUND_COMMAND      |         |         |        |          | 10          |             |          |            |             |              |
SIMPLE_COMMAND        |         |         |        |          |             |             | 11       | 11         | 11          | 11           | 1
SIMPLE_COMMAND_TAIL   | 12      | 12      | 12     | 12       |             | 12          | 13       | 13         | 13          | 13           | 1
IO_REDIRECT           |         |         |        |          |             |             | 14       | 15         | 16          | 17           | 1
```

# The table can now be used to build a lookup table.
