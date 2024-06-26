#ifndef MINISHELL_H
# define MINISHELL_H 1

/************************** INCLUDES *************************/
# include "libft.h"

# include <stdbool.h>

/************************** DEFINES *************************/
# define PS0 "" // TODO: Not required: Handle PS0
/* # define PS1 "\033[31m\\u@\\h:\\w\033[m$ " */
/* # define PS1 "\033[31m\\u@\\h:\\w\033[m\n❯ " */
# define PS1 "\033[31m\\u\033[m@\033[94m\\h\033[m@\033[92mminishell\033[m [\033[32m\\w\033[m]\n\\$ "
# define PS2 "> "
# define PS4 "+ "

# define MINISHELL_DEFAULT_XTRACEFD 2

# define MINISHELL_NAME "minishell" // TODO: USE $0

# define DEFAULT_IFS " \t\n"
# define IFS_WHITESPACE "\x09\x0a\x0b\x0c\x0d\x20" /* LC_CTYPE="en_US.UTF-8" */

# define EXIT_COMMAND_NOT_FOUND 127

# define STR_TOK_OVERRIDE     "TOK_OVERRIDE"
# define STR_TOK_OVERRIDE_ERR "TOK_OVERRIDE_ERR"
# define STR_TOK_INPUT        "TOK_INPUT"
# define STR_TOK_APPEND       "TOK_APPEND"
# define STR_TOK_APPEND_ERR   "TOK_APPEND_ERR"
# define STR_TOK_HEREDOC      "TOK_HEREDOC"
# define STR_TOK_PIPE         "TOK_PIPE"
# define STR_TOK_AND          "TOK_AND"
# define STR_TOK_OR           "TOK_OR"
# define STR_TOK_SEMI         "TOK_SEMI"
# define STR_TOK_L_PAREN      "TOK_L_PAREN"
# define STR_TOK_R_PAREN      "TOK_R_PAREN"
# define STR_TOK_SQUOTE_STR   "TOK_SQUOTE_STR"
# define STR_TOK_DQUOTE_STR   "TOK_DQUOTE_STR"
# define STR_TOK_WORD         "TOK_WORD"
# define STR_TOK_EOL          "TOK_EOL"
# define STR_TOK_ERROR        "TOK_ERROR"
# define STR_TOK_EPSILON      "TOK_EPSILON"
# define STR_TOK_UNKNOWN      "TOK_UNKNOWN"

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

# define TOK_EOL           1
# define TOK_AND           2
# define TOK_OR            3
# define TOK_PIPE          4
# define TOK_L_PAREN       5
# define TOK_R_PAREN       6
# define TOK_WORD          7
# define TOK_OVERRIDE      8
# define TOK_APPEND        9
# define TOK_INPUT        10
# define TOK_HEREDOC      11
# define TOK_EPSILON      12
# define TOK_SQUOTE_STR   13
# define TOK_DQUOTE_STR   14
# define TOK_ERROR        15
# define TOK_SEMI         16
# define TOK_OVERRIDE_ERR 17
# define TOK_APPEND_ERR   18

/* TOKEN is a TERMINAL, every other member is a NONTERMINAL */
# define COMPLETE_COMMAND      1
# define COMPLETE_COMMAND_TAIL 2
# define AND_OR                3
# define PIPE_SEQUENCE         4
# define PIPE_SEQUENCE_TAIL    5
# define COMMAND               6
# define COMPOUND_COMMAND      7
# define SIMPLE_COMMAND        8
# define SIMPLE_COMMAND_TAIL   9
# define IO_REDIRECT           10
# define TOKEN                 11
# define TREE_TYPE_UNKNOWN     12

# define NUM_NONTERMS 10
# define NUM_TOKENS 11 // TOOD: 12 (TOK_SEMI)?

# define FORCE_ANSI_C_QUOTING1 "\001\002\003\004\005\006\a\b\t\v\f\r\016\017"
# define FORCE_ANSI_C_QUOTING2 "\020\021\022\023\024\025\026\027\030\031\032"
# define FORCE_ANSI_C_QUOTING3 "\033\034\035\036\037\177"
# define FORCE_SINGLE_QUOTING " !'\"#$&()*;<>?[\\]^`{|}~"

/********************* FORWARD DECLARATIONS *********************/
typedef struct s_token			t_token;
typedef int						t_token_type;
typedef struct s_tree			t_tree;
typedef int						t_tree_type;
typedef struct s_fds			t_fds;
typedef struct s_var			t_var;
typedef struct s_flags			t_flags;
typedef struct s_declare_flags	t_declare_flags;

/*** other typedefs ***/
typedef t_tree					*t_children[];
typedef struct sigaction		t_sa;

/************************** STRUCTURES *************************/
struct s_declare_flags
{
	bool	readonly;
	bool	not_readonly;
	bool	export;
	bool	not_export;
	bool	print;
};

struct s_flags
{
	bool	exp;
	bool	readonly;
	bool	special;
	bool	unset;
};

/* must be the same as the flags of t_flags */
struct s_var
{
	bool	exp;
	bool	readonly;
	bool	special;
	char	*value;
};

struct s_token
{
	t_token_type						type;
	char								*str;
	bool								is_last_token;
	char								*quoting_ctx;
	char								*expansion_ctx;
	int									num_tokens_after_split;
	char								*escape_ctx;
	char								*origin;
};

struct s_fds
{
	int	fd_in;
	int	fd_out;
	int	fd_err;
};

/* if t_tree.type == TOKEN, then t_tree.token shall be used */
/* if t_tree.type != TOKEN, then t_tree.children
 * shall be used */
struct									s_tree
{
	t_tree_type						type;
	union
	{
		t_token						*token;
		t_list						*children;
	};
	int								error;
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

/************** PROTOTYPES ******************/
void									interactive_signals(void);
void									interactive_signals_heredoc(void);
void									noninteractive_signals(void);
t_list									*tokenize(const char *line); /* static 1 ? */
t_tree									*build_ast(t_list *tokens, bool create_heredocs);
void									tree_print(t_tree *tree);
void									print_token(t_data data, int n);
void									print_token_debug(t_data data, int n);
void									print_tree_node(void *data, int n);
void									print_tree(void *data,
											bool first);
const char								*token_type_to_string(
											t_token_type type);
void									*new_token(char *str, t_token_type type,
											bool is_last_token);
char									*get_token_str(t_list *tokens);
char									*get_token_str_nl(t_list *tokens);
t_token_type							get_token_type(t_list *tokens);
int										minishell_error(int exit_code,
											bool do_exit, bool syntax_error,
											const char *fmt, ...);
void									expand_env_vars(t_list *tokens);
/* t_list									*expand_token(t_token *token); */
t_list									*expand_tokens(t_list *tokens);
char									*expand_all_vars(char *token_str);
void									join_tokens(t_list *tokens);
char									**get_env(char *program);
char									*var_lookup(char *key);
char									**set_argv(char **argv);
char									**get_argv(void);
unsigned char							execute_complete_command(t_tree *node);
void									glob_tokens(t_list *tokens);
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
t_var									*get_var(char key[static 1]);
bool									unset_var(char key[static 1]);
void									set_var(char key[static 1],
											char *value,
											t_flags flags);
t_ht									**get_vars(void);
void									clear_vars(void);
int										builtin_cd(char **argv, t_fds fds);
int										builtin_echo(char **argv, t_fds fds);
int										builtin_env(char **argv, t_fds fds);
int										builtin_exit(char **argv, t_fds fds);
int										builtin_export(char **argv, t_fds fds);
int										builtin_readonly(char **argv,
											t_fds fds);
int										builtin_declare(char **argv, t_fds fds);
int										builtin_pwd(char **argv, t_fds fds);
int										builtin_unset(char **argv, t_fds fds);
int										builtin_source(char **argv, t_fds fds);
char									*ft_gethostname(void);
char									*ft_getusername(void);
char									*ft_getcwd(void);
t_list									*production_to_children(
											t_tree *production);
t_tree									*production_part_to_child(t_tree production);
bool									tree_is_null(t_tree *tree);
t_tree									*new_tree_token(t_token_type type,
											char *str);
t_tree									*new_tree_m_token(t_token_type type,
											const char *str);
t_tree									*new_tree_nonterm(t_tree_type type,
											t_list *children);
t_tree									*get_production(t_tree_type nonterm,
											t_token *token);
t_list									*creat_new_children(t_tree **children);
t_tree_type								tree_ht_get(char *key);
t_token_type							tokens_ht_get(char *key);
int										get_production_idx(t_tree_type nonterm,
											t_token *token);
void									finish(bool print_exit);
char									*ft_mktemp(char *prefix);
char									*create_heredoc(char *delimiter);
bool									heredoc_aborted(int op);
unsigned char							set_last_exit_status(int exit_status);
unsigned char							get_last_exit_status(void);
t_flags									get_flags(char key[static 1]);
char									*ft_getuid(void);
char									*ft_geteuid(void);
char									*ft_getpid(void);
char									*ft_getppid(void);
int										ft_getumask(void);
char									*get_ifs(void);
char									*repeat_string(char *str, size_t n);
bool									shopt_enabled(char opt);
bool									force_ansi_c_quoting(char *s);
bool									force_single_quoting(char *s);
char									*quote_single(char *s);
char									*quote_double(char *s);
char									*quote_ansi_c(char *s,
											bool bare_declare);
char									*expand_prompt(char *prompt_string);

#endif /* minishell.h. */
