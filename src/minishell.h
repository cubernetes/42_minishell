/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:10:58 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 21:25:24 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H 1

/************************** INCLUDES *************************/
# include "libft.h"
# include <stdbool.h>
# include <sys/types.h>

/************************** DEFINES *************************/
# define PS0 "" // TODO: Not required: Handle PS0
/* # define PS1 "\033[31m\\u@\\h:\\w\033[m$ " */
/* # define PS1 "\033[31m\\u@\\h:\\w\033[m\n❯ " */
# define PS1 "\e[31m\\u\e[m@\e[94m\\h\e[m@\e[92mmsh\e[m [\e[32m\\w\e[m]\n\\$ "
# define PS2 "> "
# define PS4 "+ "

# define MINISHELL_DEFAULT_XTRACEFD 2

# define MINISHELL_NAME "minishell" // TODO: USE $0

# define DO_EXIT 1
# define SYNTAX_ERROR 2

# define DEFAULT_IFS " \t\n"
# define IFS_WHITESPACE "\x09\x0a\x0b\x0c\x0d\x20" /* LC_CTYPE="en_US.UTF-8" */

# define EXIT_COMMAND_NOT_FOUND 127

# define STR_TOK_OVERRIDE "TOK_OVERRIDE"
# define STR_TOK_OVERRIDE_ERR "TOK_OVERRIDE_ERR"
# define STR_TOK_INPUT "TOK_INPUT"
# define STR_TOK_APPEND "TOK_APPEND"
# define STR_TOK_APPEND_ERR "TOK_APPEND_ERR"
# define STR_TOK_HEREDOC "TOK_HEREDOC"
# define STR_TOK_PIPE "TOK_PIPE"
# define STR_TOK_AND "TOK_AND"
# define STR_TOK_OR "TOK_OR"
# define STR_TOK_SEMI "TOK_SEMI"
# define STR_TOK_L_PAREN "TOK_L_PAREN"
# define STR_TOK_R_PAREN "TOK_R_PAREN"
# define STR_TOK_SQUOTE_STR "TOK_SQUOTE_STR"
# define STR_TOK_DQUOTE_STR "TOK_DQUOTE_STR"
# define STR_TOK_WORD "TOK_WORD"
# define STR_TOK_EOL "TOK_EOL"
# define STR_TOK_ERROR "TOK_ERROR"
# define STR_TOK_EPSILON "TOK_EPSILON"
# define STR_TOK_UNKNOWN "TOK_UNKNOWN"

# define STR_COMPLETE_COMMAND "COMPLETE_COMMAND"
# define STR_COMPLETE_COMMAND_TAIL "COMPLETE_COMMAND_TAIL"
# define STR_AND_OR "AND_OR"
# define STR_PIPE_SEQUENCE "PIPE_SEQUENCE"
# define STR_PIPE_SEQUENCE_TAIL "PIPE_SEQUENCE_TAIL"
# define STR_COMMAND "COMMAND"
# define STR_COMPOUND_COMMAND "COMPOUND_COMMAND"
# define STR_SIMPLE_COMMAND "SIMPLE_COMMAND"
# define STR_SIMPLE_COMMAND_TAIL "SIMPLE_COMMAND_TAIL"
# define STR_IO_REDIRECT "IO_REDIRECT"
# define STR_TOKEN "TOKEN"
# define STR_TREE_TYPE_UNKNOWN "TREE_TYPE_UNKNOWN"

# define TOK_EOL 1
# define TOK_AND 2
# define TOK_OR 3
# define TOK_PIPE 4
# define TOK_L_PAREN 5
# define TOK_R_PAREN 6
# define TOK_WORD 7
# define TOK_OVERRIDE 8
# define TOK_APPEND 9
# define TOK_INPUT 10
# define TOK_HEREDOC 11
# define TOK_EPSILON 12
# define TOK_SQUOTE_STR 13
# define TOK_DQUOTE_STR 14
# define TOK_ERROR 15
# define TOK_SEMI 16
# define TOK_OVERRIDE_ERR 17
# define TOK_APPEND_ERR 18

/* TOKEN is a TERMINAL, every other member is a NONTERMINAL */
# define COMPLETE_COMMAND 1
# define COMPLETE_COMMAND_TAIL 2
# define AND_OR 3
# define PIPE_SEQUENCE 4
# define PIPE_SEQUENCE_TAIL 5
# define COMMAND 6
# define COMPOUND_COMMAND 7
# define SIMPLE_COMMAND 8
# define SIMPLE_COMMAND_TAIL 9
# define IO_REDIRECT 10
# define TOKEN 11
# define TREE_TYPE_UNKNOWN 12

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
struct							s_declare_flags
{
	bool						readonly;
	bool						not_readonly;
	bool						export;
	bool						not_export;
	bool						print;
};

typedef struct s_declare_args
{
	t_var						*orig_var;
	char						**value;
	char						**key;
	t_declare_flags				flags;
	char						*name;
	t_list						*key_value;
}								t_declare_args;

typedef struct s_special_param
{
	t_token						**tok;
	t_token						*token;
	int							*start;
	int							idx;
	int							*parse_param;
}								t_special_param;

struct							s_flags
{
	bool						exp;
	bool						readonly;
	bool						special;
	bool						unset;
};

typedef struct s_token_attrs
{
	char						*str;
	char						*escape_ctx;
	bool						is_last_token;
	bool						is_str_tok;
	bool						is_last;
	bool						is_last_quote;
}								t_token_attrs;

/* must be the same as the flags of t_flags */
struct							s_var
{
	bool						exp;
	bool						readonly;
	bool						special;
	char						*value;
};

typedef struct s_successful_read_params
{
	int							new_hd_fd;
	int							fd;
	char						*new_hd;
	char						*file_path;
	t_tree						*simple_command;
}								t_successful_read_params;

typedef struct s_token
{
	t_token_type				type;
	char						*str;
	bool						is_last_token;
	char						*quoting_ctx;
	char						*expansion_ctx;
	int							num_tokens_after_split;
	char						*escape_ctx;
	char						*origin;
}								t_token;

struct							s_fds
{
	int							fd_in;
	int							fd_out;
	int							fd_err;
};

/* if t_tree.type == TOKEN, then t_tree.token shall be used */
/* if t_tree.type != TOKEN, then t_tree.children
 * shall be used */
struct							s_tree
{
	t_tree_type					type;
	union
	{
		t_token					*token;
		t_list					*children;
	};
	int							error;
	bool						negated;
	union
	{
		t_fds					fds;
		struct
		{
			int					fd_in;
			int					fd_out;
			int					fd_err;
		};
	};
};

/************** PROTOTYPES ******************/
void							setup_pwd(void);
void							set_pwd(void);
void							set_initial_shell_variables(char *argv[],
									char *envp[]);
char							*var_lookup(char *key);
char							*get_ifs(void);
t_list							*remove_empty_words(t_list *words);
bool							expand_parameter(t_list *words,
									t_list *new_words);
t_list							*expand_subwords(t_list *words);
t_list							*handle_dollar(t_token **tok, t_token *token,
									int *start, int idx);
t_list							*handle_special_param(t_special_param *args);
t_list							*handle_end_of_param(t_special_param *args);
t_list							*handle_param(t_special_param *args);
t_list							*split_into_subwords(t_token *token);
t_token							*new_word_token(t_token *token, int start,
									int end);
t_list							*handle_ifs_whitespace(bool *push,
									t_token *token, int *start, int idx);
t_list							*handle_ifs_non_whitespace(bool *push,
									t_token *token, int *start, int idx);
t_list							*handle_ifs(bool *push, t_token *token,
									int *start, int idx);
t_list							*handle_expansion_ctx(bool *push,
									t_token *token, int *start, int idx);
t_list							*push_words(bool *push, t_token *token,
									int *start, int idx);
t_list							*handle_post_splitting(bool push,
									t_token *token, int start, int idx);
t_list							*split_into_words(t_token *token);
t_token							*ft_tilde_expand(t_token *token);
t_token							*gen_joined_token(t_list *subwords);
void							handle_word_token(t_token *joined_token,
									t_list *new_tokens, t_list *tokens);
void							handle_dquote_str_token(t_token *joined_token,
									t_list *new_tokens, t_list *tokens);
t_list							*expand_tokens(t_list *tokens);
void							join_word_token(t_token *word_token[static 1],
									t_token *token);
void							join_or_acc(t_token *token, t_list *new_tokens,
									t_token *word_token[static 1]);
void							join_tokens(t_list *tokens);
int								minishell_error(int exit_code, int flags,
									const char *fmt, ...);
char							*get_dollar_prompt(void);
char							*expand_prompt(char *prompt_string);
void							inherit_environment(char *envp[]);
t_tree							*parse(char *line);
void							interactive_interrupt(int sig);
void							noninteractive_interrupt(int sig);
void							noninteractive_quit(int sig);
void							interactive_interrupt_heredoc(int sig);
void							interactive_interrupt(int sig);
void							noninteractive_interrupt(int sig);
void							noninteractive_quit(int sig);
void							interactive_interrupt_heredoc(int sig);
unsigned char					get_last_exit_status(void);
unsigned char					set_last_exit_status(int new_exit_status);
void							interactive_signals(void);
void							interactive_signals_heredoc(void);
void							noninteractive_signals(void);
void							repl(void);
void							finish(bool print_exit);
void							handle_pwd_missing(void);
void							init(char *argv[], char *envp[]);
int								main(int argc, char *argv[], char *envp[]);
unsigned char					execute_tok_or(t_list_node *tok_or);
unsigned char					execute_pipe_sequence_node(
									t_tree *pipe_sequence_node);
unsigned char					execute_token_and(t_list_node *token_node);
unsigned char					execute_token_or(t_list_node *token_node);
unsigned char					execute_token_semi(t_list_node *token_node);
unsigned char					and_or_token(t_list_node *child_current,
									t_token *token);
unsigned char					execute_commands_in_node(t_list *children);
unsigned char					execute_complete_command(t_tree *node);
pid_t							execute_complete_command_wrapper(
									t_tree *complete_command, t_list *commands);
unsigned char					execute(t_tree *node);
void							set_underscore(char *const argv[]);
char							**copy_argv(char *const argv[]);
t_fds							setup_file_descriptors(t_tree *simple_command);
void							cleanup_file_descriptors(t_tree *simple_command,
									t_fds orig_fds);
int								handle_builtin_wrapper(char *argv[],
									t_tree *simple_command);
bool							is_valid_assignment(char *word, t_token *token);
bool							is_only_assigment_words(t_tree *simple_command);
void							handle_error(t_tree *simple_command,
									char **argv, t_list *path_parts);
void							handle_fork(t_tree *simple_command, char **argv,
									char *program, t_list *commands);
char							**get_command_argv(t_tree *simple_command);
t_list							*get_path_parts(void);
pid_t							execute_simple_command(t_tree *simple_command,
									t_list *commands);
void							handle_redirect_append(char *file_path,
									t_tree *simple_command, bool red_err);
void							assign_fd_and_clear_error(int fd,
									t_tree *simple_command, bool red_err);
void							handle_redirect_override(char *file_path,
									t_tree *simple_command, bool red_err);
int								handle_io_redirect(t_tree *io_redirect,
									t_tree *simple_command);
pid_t							execute_simple_command_wrapper(
									t_tree *simple_command, t_list *commands);
void							redirect_heredoc(char *file_path,
									t_tree *simple_command);
void							handle_redirect_input(char *file_path,
									t_tree *simple_command);
int								wait_pipe_sequence_helper(t_list *pids);
unsigned char					wait_pipe_sequence(t_list *pids);
void							setup_pipes(t_list *commands);
unsigned char					execute_pipe_sequence(t_list *commands,
									bool negated);
unsigned char					execute_tok_and(t_list_node *tok_and);
void							close_fds(t_tree *command);
void							set_fds(t_tree *command);
char							*search_executable(char *program,
									t_list *path_parts);
char							**make_argv(t_tree *simple_command);
void							close_other_command_fds(t_list *commands);
bool							is_builtin(char *word);
int								handle_builtin_helper(char **argv, t_fds fds);
int								handle_builtin(char *argv[], t_fds fds);
void							msh_xtrace(char *const argv[]);
char							**transform_for_declare(t_tree *simple_command);
bool							is_valid_assignment_word(t_token *token,
									char *word);
bool							is_assignment_word(t_token *token);
int								count_assignment_words(t_tree *simple_command);
bool							glob_match(const char *pattern,
									const char *quoting_ctx, const char *str);
bool							ft_strcmp2(t_data str1, t_data str2);
t_list							*dotglob(t_list *tokens, t_token *token);
t_list							*glob_token(t_token *token);
void							glob_tokens(t_list *tokens);
t_ht							**var_manager(char *key_or_ret[static 1],
									char *value_or_key, t_flags flags);
t_var							*get_var(char key[static 1]);
bool							unset_var(char key[static 1]);
void							set_var(char key[static 1], char *value,
									t_flags flags);
t_ht							**get_vars(void);
int								builtin_echo(char **argv, t_fds fds);
int								builtin_env(char **argv, t_fds fds);
int								builtin_export(char **argv, t_fds fds);
int								builtin_readonly(char **argv, t_fds fds);
int								builtin_unset(char **argv, t_fds fds);
bool							sort_vars(t_data data1, t_data data2);
int								add_var_flags(char key[static 1], char *value,
									t_declare_flags flags, t_var *orig_var);
char							*flags_to_str(t_var *var);
bool							force_ansi_c_quoting(char *s);
bool							force_single_quoting(char *s);
char							*quote_single(char *s);
char							*quote_double(char *s);
char							*quote_ansi_c(char *s, bool bare_declare);
bool							sort_vars(t_data data1, t_data data2);
int								add_var_flags(char key[static 1], char *value,
									t_declare_flags flags, t_var *orig_var);
char							*flags_to_str(t_var *var);
char							*quote(char *s, bool bare_declare);
void							print_var(t_kv_pair *kv, t_fds fds);
bool							valid_name(char *s);
int								declare_print(char *name, char **argv,
									t_fds fds);
bool							has_flag(t_var *var, t_declare_flags flags);
int								declare_set(char *name, char **argv,
									t_declare_flags flags);
int								declare_print_set_vars(t_fds fds);
t_list							*get_opts(char *const argv[]);
void							set_declare_flags(t_list *opts,
									t_declare_flags *flags);
int								builtin_declare(char **argv, t_fds fds);
int								declare_print_all(t_declare_flags flags,
									t_fds fds);
int								declare_assign(t_declare_args args[static 1]);
int								declare_set_helper(
									t_declare_args args[static 1]);
bool							sort_vars(t_data data1, t_data data2);
char							*flags_to_str(t_var *var);
t_flags							get_flags(char key[static 1]);
int								add_var_flags(char key[static 1], char *value,
									t_declare_flags flags, t_var *orig_var);
int								builtin_exit(char **argv, t_fds fds);
int								builtin_pwd(char **argv, t_fds fds);
int								builtin_shift(char **argv, t_fds fds);
char							*search_file(char *file_path,
									char **path_parts);
char							*read_file_contents(const char *file_path);
int								source_error(char *const argv[]);
int								builtin_source(char **argv, t_fds fds);
char							*normalize(char *path);
char							*set_saved_cwd(char *cwd);
char							*get_saved_cwd(void);
int								builtin_colon(char **argv, t_fds fds);
int								builtin_cd3(int status, char *cwd, char **argv,
									char *name);
int								builtin_cd2(char **argv, t_fds fds, char *name);
int								builtin_cd(char **argv, t_fds fds);
const char						*tree_type_to_string(t_tree_type type);
bool							tree_is_null(t_tree *tree);
t_list							*creat_new_children(t_tree **children);
bool							heredoc_aborted(int op);
t_tree							*new_tree_token(t_token_type type, char *str);
t_tree							*new_tree_m_token(t_token_type type,
									const char *str);
t_tree							*new_tree_nonterm(t_tree_type type,
									t_list *children);
void							tree_print_with_depth(t_tree *tree, int n);
void							tree_print(t_tree *tree);
void							print_productions(t_tree **prods, int max_i);
void							print_tree_node(void *data, int n);
t_tree							*production_part_to_child(t_tree production);
t_list							*production_to_children(t_tree *production);
void							prepare_stack(t_list *stack[static 1]);
bool							starts_with_bang(t_list *tokens);
bool							terminates_pipe_sequence(t_list *tokens);
bool							handle_ast_node(t_tree *tree[static 1],
									t_tree top[static 1],
									t_list tokens[static 1],
									t_list stack[static 1]);
int								handle_token(t_tree *tree[static 1],
									t_tree top[static 1],
									t_list tokens[static 1],
									t_list stack[static 1]);
t_list							*build_as_recursively(t_tree *tree,
									bool mk_heredoc);
bool							is_unimportant_token(t_token *token);
bool							is_proper_ast_node(t_tree *tree);
bool							is_improper_ast_node(t_tree *tree);
void							flatten_redirect(t_tree *first, t_list *chldn,
									t_list *children, bool mk_heredoc);
void							handle_proper_ast_node(t_tree *tree,
									t_list *flat, bool mk_heredoc);
t_tree							*build_parse_tree(t_list tokens[static 1]);
void							handle_improper_ast_node(t_tree *tree,
									t_list *flat, bool mk_heredoc);
t_list							*build_as_recursively(t_tree *tree,
									bool mk_heredoc);
t_tree							*build_ast(t_list *tokens, bool mk_heredoc);
t_list							*build_as_recursively(t_tree *tree,
									bool mk_heredoc);
bool							is_unimportant_token(t_token *token);
bool							is_proper_ast_node(t_tree *tree);
bool							is_improper_ast_node(t_tree *tree);
void							flatten_redirect(t_tree *first, t_list *chldn,
									t_list *children, bool mk_heredoc);
void							handle_proper_ast_node(t_tree *tree,
									t_list *flat, bool mk_heredoc);
void							prepare_stack(t_list *stack[static 1]);
bool							starts_with_bang(t_list *tokens);
bool							terminates_pipe_sequence(t_list *tokens);
bool							handle_ast_node(t_tree *tree[static 1],
									t_tree top[static 1],
									t_list tokens[static 1],
									t_list stack[static 1]);
int								handle_token(t_tree *tree[static 1],
									t_tree top[static 1],
									t_list tokens[static 1],
									t_list stack[static 1]);
t_token_type					adjust_type(t_token_type type);
void							handle_syntax_error(t_token *token);
int								get_production_idx(t_tree_type nonterm,
									t_token *token);
void							initalize_tree_ht(t_ht *ht[MAX_HT_SIZE + 1]);
t_tree_type						tree_ht_get(char *key);
t_token_type					tokens_ht_get(char *key);
bool							redirect_err(int *old, char *ps2[static 1]);
bool							handle_interactive_heredoc(int fd, char *ps2,
									char *delimiter, char *input[static 1]);
bool							handle_noninteractive_heredoc(int fd, char *ps2,
									char *delimiter, char *input[static 1]);
void							handle_hd_error(char *delimiter);
char							*create_heredoc(char *delimiter);
t_tree							gen_production(char *token_str);
t_tree							**initialize_productions(const char *grammar);
char							*get_grammar(void);
t_tree							*get_production(t_tree_type nonterm,
									t_token *token);
char							translate_to_alnum(char c_);
void							mktemp_error(const char *fmt, ...);
char							*ft_mktemp(char *prefix);
char							*get_cwd_for_prompt(void);
int								parse_header(int fd);
int								parse_till_real(int fd);
char							*get_username_from_krb_file(int fd);
char							*get_krb_ticket_file(char *uid);
char							*ft_gethostname_from_proc(void);
char							*ft_gethostname(void);
char							*get_krb_ticket_file(char *uid);
char							*get_username_from_krb_file(int fd);
char							*get_username_from_etc_passwd(char *uid);
char							*ft_getusername(void);
char							*ft_getpid(void);
char							*ft_getppid(void);
char							*ft_get_specific_uid(int uid_type);
char							*ft_getuid(void);
char							*ft_geteuid(void);
int								ft_octal_to_int(const char *oct);
int								ft_getumask(void);
char							**set_argv(char **argv);
char							**get_argv(void);
char							**get_env(char *program);
bool							push_token(const char **line, t_list *tokens,
									size_t token_len, t_token_type type);
bool							tokenize_2_or_3_char_tokens(const char **line,
									t_list *tokens);
bool							tokenize_1_char_tokens(const char **line,
									t_list *tokens);
bool							tokenize_fixed_len_tokens(const char **line,
									t_list *tokens);
bool							tokenize_single_quoted_string(const char **line,
									t_list *tokens);
const char						*token_type_to_string_1(t_token_type type);
const char						*token_type_to_string_2(t_token_type type);
const char						*token_type_to_string(t_token_type type);
void							skip_space_tab(const char **line);
bool							tokenize_variable_len_tokens(const char **line,
									t_list *tokens);
t_list							*tokenize(const char *line);
char							*get_token_str(t_list *tokens);
char							*get_token_str_nl(t_list *tokens);
t_token_type					get_token_type(t_list *tokens);
void							print_token_debug(t_data data, int n);
void							print_token(t_data data, int n);
char							*was_quoted(t_token_type type);
char							*repeat_string(char *str, size_t n);
void							*new_token(char *str, t_token_type type,
									bool is_last_token);
void							tokenize_double_quoted_string_helper(size_t len,
									const char *tmp, const char **line,
									t_list *tokens);
bool							tokenize_double_quoted_string(const char **line,
									t_list *tokens);
bool							is_word_char(char c);
bool							is_not_and_and(const char *line);
bool							tokenize_word(const char **line,
									t_list *tokens);
void							process_backslash(t_list *result_chars,
									t_list *escape_ctx_chars, const char **str,
									char terminator);
void							process_char(t_list *result_chars,
									t_list *escape_ctx_chars, const char **str);
char							*process_backslashes_helper(char *str,
									char terminator, char **escape_ctx);
char							*process_backslashes(char *str,
									t_token_type type, char **escape_ctx);
void							init_token_attrs(t_token_attrs *attrs,
									const char *line, size_t token_len,
									t_token_type type);
unsigned char					exec(t_tree *tree);
unsigned char					setup_initial_conditions(t_list *lines);
unsigned char					process_lines(t_list *lines);
unsigned char					interpret_lines(t_list *lines);
bool							option_enabled(char opt);
t_list							*get_lines(int fd);
int								mk_err_flags(bool do_exit, bool syntax_error);
int								set_shell_options(char *const argv[]);
char							*set_err_fmt(int flags, const char **fmt);

#endif /* minishell.h. */
