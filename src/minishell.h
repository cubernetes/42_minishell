#ifndef MINISHELL_H
# define MINISHELL_H 1

# include "libft.h"

# include <stdbool.h>

# define PS0 "> "
/* # define PS1 "\033[31m\\u@\\h:\\w\033[m\n❯ " */
# define PS1 "\033[31m\\u\033[m@\033[94m\\h\033[m@\033[92mminishell\033[m [\033[32m\\w\033[m]\n> "

# define MINISHELL_NAME "minishell"

# define IFS " \n\t"

/* Exit codes */
# define EXIT_COMMAND_NOT_FOUND 127

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
# define STR_TREE_TYPE_UNKNOWN     "TREE_TYPE_UNKNOWN"

# define NUM_NONTERMS 10
# define NUM_TOKENS 11

typedef struct s_str_pair
{
	char	*l;
	char	*r;
}			t_str_pair;

typedef struct s_flags
{
	bool	exp;
	bool	hidden;
	bool	readonly;
}	t_flags;

typedef struct s_var
{
	bool	exp;
	bool	readonly;
	bool	hidden;
	char	*value;
}	t_var;

typedef struct sigaction	t_sa;

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
}										t_token_type;

typedef struct s_token					t_token;
typedef struct s_token
{
	t_token_type						type;
	char								*str;
	bool								is_last_subtoken;
}										t_token;

typedef struct s_tree				t_tree;

typedef t_tree						*t_children[];

/* TOKEN is a TERMINAL, every other member is a NONTERMINAL */
typedef enum e_tree_type
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
	TREE_TYPE_UNKNOWN
}										t_tree_type;

typedef struct s_fds
{
	int	fd_in;
	int	fd_out;
	int	fd_err;
}	t_fds;

/* if t_tree.type == TOKEN, then t_tree.token shall be used */
/* if t_tree.type != TOKEN, then t_tree.children
 * shall be used */
struct									s_tree
{
	t_tree_type						type;
	union
	{
		t_token							*token;
		t_list							*children;
	};
	union
	{
		t_fds	fds;
		struct
		{
			int	fd_in;
			int	fd_out;
			int	fd_err;
		};
	};
};

void									setup_signals(void);
t_list									*tokenize(const char *line); /* static 1 ? */
t_tree									*build_ast(t_list *tokens);
void									tree_print(t_tree *tree);
void									print_token(void *data, bool first);
void									print_tree_node(void *data, bool first);
void									print_tree(void *data,
											bool first);
const char								*token_type_to_string(
											t_token_type type);
void									*new_token(char *str, t_token_type type,
											bool is_last_subtoken);
char									*get_token_str(t_list *tokens);
char									*get_token_str_nl(t_list *tokens);
t_token_type							get_token_type(t_list *tokens);
int										minishell_error(int exit_code,
											bool do_exit,
											const char *fmt, ...);
void									expand_env_vars(t_list *tokens);
void									join_tokens(t_list *tokens);
char									**set_env(char **envp);
char									**get_env(void);
char									*env_lookup(char *var);
char									**set_argv(char **argv);
char									**get_argv(void);
unsigned char							execute(t_tree *tree);
void									glob_tokens(t_list *tokens);
t_type									ht_get(t_ht *ht[MAX_HT_SIZE],
											char key[static 1]);
void									ht_set(t_ht *ht[MAX_HT_SIZE],
											char key[static 1],
											t_type value);
void									ht_print(t_ht ht[MAX_HT_SIZE],
											void (print)(char k[static 1],
												void *v));
void									ht_destroy(t_ht *ht[MAX_HT_SIZE]);
int										ms_execve(t_tree *command);
const char								*tree_type_to_string(
											t_tree_type type);
pid_t									execute_simple_command(
											t_tree *simple_command,
											t_list *commands);
pid_t									execute_simple_command(
											t_tree *simple_command,
											t_list *commands);
void									close_fds(t_tree *simple_command);
void									set_fds(t_tree *simple_command);
void									close_other_command_fds(
											t_list *commands);
char									*set_var(char *key, char *value,
											t_flags flags);
char									*get_var(char *key);
void									clear_vars(void);
int										builtin_cd(char **argv, t_fds fds);
int										builtin_echo(char **argv, t_fds fds);
int										builtin_env(char **argv, t_fds fds);
int										builtin_exit(char **argv, t_fds fds);
int										builtin_export(char **argv, t_fds fds);
int										builtin_pwd(char **argv, t_fds fds);
int										builtin_unset(char **argv, t_fds fds);
char									*ft_gethostname(void);
char									*ft_getcwd(void);
t_list									*productions_to_children(
											t_tree *productions);
t_tree									*production_to_child(t_tree production);
bool									tree_is_null(t_tree *tree);
t_tree									*new_tree_token(t_token_type type,
											char *str);
t_tree									*new_tree_m_token(t_token_type type,
											const char *str);
t_tree									*new_tree_nonterm(t_tree_type type,
											t_list *children);
t_tree									*get_production(t_tree_type nonterm,
											t_token *token);
char									*ft_gethostname(void);
char									*ft_getcwd(void);
t_list									*new_children(t_tree **children);
t_tree_type								tree_ht_get(char *key);
t_token_type							tokens_ht_get(char *key);
int										get_production_idx(t_tree_type nonterm,
											t_token *token);

#endif /* minishell.h. */
