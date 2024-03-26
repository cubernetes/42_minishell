#ifndef MINISHELL_H
# define MINISHELL_H	1

/* TODO: remove debug include */
# include "../libft/libft.h"
# include "minishell_debug.h"
# define PS0 "> "
/* TODO: use advanced prompt */
/* # define PS1 "\\u@\\h:\\w\\$ " */
# define PS1 "$ "
# define IFS 32

# define STR_TOK_OVERRIDE   "TOK_OVERRIDE"
# define STR_TOK_INPUT      "TOK_INPUT"
# define STR_TOK_APPEND     "TOK_APPEND"
# define STR_TOK_HEREDOC    "TOK_HEREDOC"
# define STR_TOK_PIPE       "TOK_PIPE"
# define STR_TOK_AND        "TOK_AND"
# define STR_TOK_OR         "TOK_OR"
# define STR_TOK_L_PAREN    "TOK_L_PAREN"
# define STR_TOK_R_PAREN    "TOK_R_PAREN"
# define STR_TOK_SQUOTE_STR "TOK_SQUOTE_STR"
# define STR_TOK_DQUOTE_STR "TOK_DQUOTE_STR"
# define STR_TOK_WORD       "TOK_WORD"
# define STR_TOK_EOL        "TOK_EOL"
# define STR_TOK_ERROR      "TOK_ERROR"
# define STR_TOK_EPSILON    "TOK_EPSILON"
# define STR_TOK_UNKNOWN    "TOK_UNKNOWN"

# define STR_COMPLETE_COMMAND      "COMPLETE_COMMAND"
# define STR_COMPLETE_COMMAND_TAIL "COMPLETE_COMMAND_TAIL"
# define STR_AND_OR                "AND_OR"
# define STR_PIPE_SEQUENCE         "PIPE_SEQUENCE"
# define STR_PIPE_SEQUENCE_TAIL    "PIPE_SEQUENCE_TAIL"
# define STR_COMMAND               "COMMAND"
# define STR_COMPOUND_COMMAND      "COMPOUND_COMMAND"
# define STR_SIMPLE_COMMAND        "SIMPLE_COMMAND"
# define STR_SIMPLE_COMMAND_TAIL   "SIMPLE_COMMAND_TAIL"
# define STR_IO_REDIRECT           "IO_REDIRECT"
# define STR_TOKEN                 "TOKEN"
# define STR_AST_NODE_TYPE_UNKNOWN "AST_NODE_TYPE_UNKNOWN"

# define NUM_NONTERMS 10
# define NUM_TOKENS   11

typedef struct s_state
{
	char	*ps0;
	char	*ps1;
}			t_state;

typedef enum e_token_type
{
	TOK_EOL = 1,
	TOK_AND,
	TOK_OR,
	TOK_PIPE,
	TOK_L_PAREN,
	TOK_R_PAREN,
	TOK_WORD,
	TOK_OVERRIDE,
	TOK_APPEND,
	TOK_INPUT,
	TOK_HEREDOC,
	TOK_EPSILON,
	TOK_SQUOTE_STR,
	TOK_DQUOTE_STR,
	TOK_ERROR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*str;
	t_bool			is_last_subtoken;
}	t_token;

typedef struct s_ast_node	t_ast_node;
typedef t_ast_node			*t_children[];

/* TODO: NOT REQUIRED: int err; */
typedef struct s_fds
{
	int	in;
	int	out;
}	t_fds;

typedef struct s_simple_command
{
	t_fds			fds;
	unsigned char	exit_status;
}	t_simple_command;

typedef union u_ast_meta_data
{
	t_simple_command	simple_command;
	t_token_type		conditional;
}	t_ast_meta_data;

/* children is a NULL terminated array of t_ast_node ptrs */
typedef union u_ast_node_data
{
	t_token		*token;
	t_ddeque	*children;
}	t_ast_node_data;

/* TOKEN is a TERMINAL, every other member is a NONTERMINAL */
typedef enum e_ast_node_type
{
	COMPLETE_COMMAND = 1,
	COMPLETE_COMMAND_TAIL,
	AND_OR,
	PIPE_SEQUENCE,
	PIPE_SEQUENCE_TAIL,
	COMMAND,
	COMPOUND_COMMAND,
	SIMPLE_COMMAND,
	SIMPLE_COMMAND_TAIL,
	IO_REDIRECT,
	TOKEN,
	AST_NODE_TYPE_UNKNOWN
}	t_ast_node_type;

typedef	struct s_fdrd
{
	int	fd_stdin;
	int	fd_stdout;
	int	fdin;
	int	fdout;
	char	*infile;
	char	*outfile;
	char	*herdoc_delimiter;
}	t_fdrd;


/* if t_ast_node.type == TOKEN, then t_ast_node.data.token shall be used */
/* if t_ast_node.type != TOKEN, then t_ast_node.data.children
 * shall be used */
struct s_ast_node
{
	t_ast_node_type	type;
	t_ast_node_data	data;
	t_ast_meta_data	meta_data;
};

typedef t_ast_node			t_production;

void			setup_signals(void);
t_ddeque		*tokenize(const char *line);
t_ast_node		*build_ast(t_ddeque *tokens);
void			ast_print(t_ast_node *ast_node);
void			print_token(void *data, t_bool first);
const char		*token_type_to_string(t_token_type type);
void			*new_token(char *str, t_token_type type,
					t_bool is_last_subtoken);
char			*get_token_str(t_ddeque *tokens);
char			*get_token_str_nl(t_ddeque *tokens);
t_token_type	get_token_type(t_ddeque *tokens);
void			minishell_error(int exit_code, const char *fmt, ...);
void			expand_env_vars(t_ddeque *tokens);
void			join_tokens(t_ddeque *tokens);
char			**set_environ(char **envp);
char			**get_environ(void);
char			*env_lookup(char *var);
void			execute(t_ast_node *ast_node);
void			glob_tokens(t_ddeque *tokens);
#endif /* minishell.h. */
