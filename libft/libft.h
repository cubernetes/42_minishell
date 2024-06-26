#ifndef LIBFT_H
# define LIBFT_H 1

/***************** INCLUDES *****************/
# include <stdarg.h>
# include <stddef.h>
# include <sys/types.h>
# include <stdbool.h>
# include <stdint.h>

/***************** DEFINES *****************/
# define HEX_DIGITS "0123456789abcdef"
# define UHEX_DIGITS "0123456789ABCDEF"
# define NULL_PTR_STR "(null)"
# define NIL_PTR_STR "(nil)"

# define EMPTY_LIST "Empty list.\n"

# define UNIQUE1 "<START_UNIQUE>"
# define UNIQUE2 "<END_UNIQUE>"

# define OFFSET_BASIS_64  14695981039346656037UL
# define FNV_PRIME_64  1099511628211UL
# define MAX_HT_SIZE 1000
/***************** FORWARD DECLARATIONS. *****************/
typedef struct s_list			t_list;
typedef struct s_list_node		t_list_node;
typedef struct s_token			t_token;
typedef int						t_token_type;
typedef struct s_tree			t_tree;
typedef int						t_tree_type;
typedef struct s_var			t_var;
typedef struct s_literator		t_literator;
typedef struct s_data			t_data;
typedef struct s_fatptr			t_fatptr;
typedef struct s_ht				t_ht;
typedef struct s_str_pair		t_str_pair;
typedef struct s_kv_pair		t_kv_pair;
typedef struct s_ctx_meta		t_ctx_meta;

/***************** ENUMS *****************/
/** Comprehensive enumeration of data types, must match union members of t_data.
 */
enum e_type
{
	TYPE_INT,
	TYPE_SIZE_T,
	TYPE_PID_T,
	TYPE_GETOPT_ARG,
	TYPE_CHAR,
	TYPE_STR,
	TYPE_PTR,
	TYPE_GC_PTR,
	TYPE_LITERATOR,
	TYPE_TOKEN,
	TYPE_TOKEN_TYPE,
	TYPE_TREE,
	TYPE_TREE_TYPE,
	TYPE_VAR,
	TYPE_LIST,
	TYPE_STR_PAIR,
	TYPE_KV_PAIR,
};

enum						e_method
{
	GET_CONTEXT,
	SET_CONTEXT,
	DESTROY_CONTEXT,
	DESTROY_CURRENT_CONTEXT,
	DESTROY_ALL_CONTEXTS,
};

typedef enum e_method			t_method;

/***************** STRUCTURES *****************/
struct						s_ctx_meta
{
	t_ht					*(*ctxs)[MAX_HT_SIZE];
	char					*ctx_name;
	t_list					*ctx;
};

struct						s_fatptr
{
	void	*data;
	size_t	size;
};

/** Data structure to be used with generic data structure implementations.
 */
struct s_data
{
	enum e_type		type;
	union
	{
		int				as_int;
		size_t			as_size_t;
		pid_t			as_pid_t;
		int				as_getopt_arg;
		char			as_char;
		char			*as_str;
		void			*as_ptr;
		void			*as_gc_ptr;
		t_literator		*as_literator;
		t_token			*as_token;
		t_token_type	as_token_type;
		t_tree			*as_tree;
		t_tree_type		as_tree_type;
		t_var			*as_var;
		t_list			*as_list;
		t_str_pair		*as_str_pair;
		t_kv_pair		*as_kv_pair;
	};
};

struct s_kv_pair
{
	char	*k;
	t_data	v;
};

struct s_str_pair
{
	char	*l;
	char	*r;
};

/** List node structure for `next` and `prev' pointers
 *  and anonymous union that MUST be identical to t_data
 *  and MUST come as the first member of the structure.
 */
struct s_list_node
{
	struct
	{
		enum e_type		type;
		union
		{
			int				as_int;
			size_t			as_size_t;
			pid_t			as_pid_t;
			int				as_getopt_arg;
			char			as_char;
			char			*as_str;
			void			*as_ptr;
			void			*as_gc_ptr;
			t_literator		*as_literator;
			t_token			*as_token;
			t_token_type	as_token_type;
			t_tree			*as_tree;
			t_tree_type		as_tree_type;
			t_var			*as_var;
			t_list			*as_list;
			t_str_pair		*as_str_pair;
			t_kv_pair		*as_kv_pair;
		};
	};
	t_list_node	*next;
	t_list_node	*prev;
};

/** List iterator structure for internal use
 */
struct s_literator
{
	t_list_node	*current;
	size_t		current_idx;
	t_list_node	*(*method)(t_list *list);
};

/** List structure with `first' and `last' pointers,
 *  `len', and `current' for iteration. Other fields are private.
 */
struct s_list
{
	size_t			len;
	t_list_node		*first;
	t_list_node		*last;
	t_list_node		*current;
	size_t			current_idx; /* internal */
	t_list_node		*(*_method)(t_list *list); /* internal */
	t_list			*_iterator_stack; /* internal, don't iterate this one! */
};

typedef struct s_ht
{
	char		*k;
	t_data		v;
	t_ht		*n;
}				t_ht;

/***************** PROTOTYPES *****************/
/* memory */
void							*ft_memmove(void *dest, void const *src,
									size_t n);
void							*ft_calloc(size_t nmemb, size_t size);
void							*ft_memset(void *s, int c, unsigned long n);
void							ft_bzero(void *s, size_t n);
void							*ft_memchr(const void *s, int c, size_t n);
int								ft_memcmp(void const *s1, void const *s2,
									size_t n);
void							*ft_memcpy(void *dest, void const *src,
									size_t n);
void							*ft_memdup(const void *src, size_t size);
char							*strip_nul(char	*bytes, size_t size);

/* gc */
bool							dont_free(void *ptr);
bool							ft_free(void *ptr);
bool							gc_free(char *ctx);
bool							gc_free_all(void);
char							*gc_add_str(void *ptr);
t_list							*gc_add(void *ptr);
t_list							*gc_get_context(void);
char							*gc_get_context_name(void);
t_list							*gc_set_context(char *new_ctx_name);
char							*gc_start_context(char *new_ctx_name);
char							*gc_end_context(void);
t_ctx_meta						gc_ctx_manager(t_method method, char *_ctx_name);
void							*ft_malloc(size_t size);
void							*gc_malloc(size_t size);
void							*(*get_allocator(void))(size_t size);
void							*(*set_allocator(void *(*_allocator)
										(size_t size)))(size_t size);

/* strings */
int								ft_isalnum(int c);
int								ft_isprint(int c);
int								ft_isascii(int c);
int								ft_isspace(int c);
int								ft_isalpha(int c);
int								ft_isdigit(int c);
char							*ft_strnstr(char const *big, char const *little,
									size_t len);
char							*ft_strchr(char const *s, int c);
char							*ft_strrchr(char const *s, int c);
char							*ft_strdup(char const s[static 1]);
char							*ft_nullable_strdup(char const *s);
char							*ft_strndup(char const *s, size_t len);
int								ft_strncmp(char const *s1, char const *s2,
									size_t n);
int								ft_strcmp(char const s1[static 1],
									char const s2[static 1]);
int								ft_nullable_strcmp(char const *s1,
									char const *s2);
int								ft_streq(char const *s1, char const *s2);
size_t							ft_strlen(char const s[static 1]);
int								ft_toupper(int c);
int								ft_tolower(int c);
char							*ft_strtrim(char const *s1, char const *set);
size_t							ft_strlcat(char *dst, char const *src,
									size_t size);
size_t							ft_strlcat(char *dst, char const *src,
									size_t size);
size_t							ft_strlcpy(char *dst, char const *src,
									size_t size);
char							*ft_strmapi(char const *s, char (f)(
										unsigned int, char));
void							ft_striteri(char *s, void (f)(unsigned int,
										char *));
int								ft_char_in_charset(char c, char const *charset);

/* string++ */
char							**ft_split(char const *s, char c);
char							*ft_strjoin(char const *s1, char const *s2);
char							*ft_substr(char const *s, unsigned int start,
									size_t len);
char							*uniquize(const char str[static 1]);
size_t							ft_count_all(const char str[static 1],
									const char pattern[static 1]);
char							*ft_replace(const char str[static 1],
									const char pattern[static 1],
									const char replacement[static 1]);
char							*ft_replace_all(const char str[static 1],
									const char pattern[static 1],
									const char replacement[static 1]);

/* math */
bool							cmp_int_asc(int a, int b);
bool							cmp_int_desc(int a, int b);
char							*ft_itoa(int n);
int								ft_atoi(char const *nptr);
int								ft_atoi_status(char const *nptr, int *status);
long 							ft_atol_status(char const *nptr, int *status);
double							ft_atof(const char *s);
unsigned int					ft_abs(int n);
int								ft_max(int a, int b);
int								ft_min(int a, int b);

/* io */
char							*get_next_line(int fd);
t_fatptr						get_next_fat_line(int fd);
int								ft_putendl_fd(char *s, int fd);
int								ft_putnbr_fd(int nb, int fd);
int								ft_putchar_fd(char c, int fd);
int								ft_putstr_fd(char *s, int fd);
int								ft_puthex_fd(unsigned long nbr, int uppercase,
									int fd);
int								ft_puthex(unsigned long nbr, int uppercase);
int								ft_putnbr(int nb);
int								ft_putchar(char c);
int								ft_putstr(char *s);
void							ft_putstr_n(const char *str, int n);
void							ft_putstr_n_clr(const char *str, int n,
									bool color);
int								ft_putptr(void *ptr);
int								ft_putptr_fd(void *ptr, int fd);
int								ft_putfmt(const char **fmt, va_list ap);
int								ft_putfmt_fd(int fd, const char **fmt,
									va_list ap);
int								ft_vprintf(const char *fmt, va_list ap);
int								ft_printf(const char *fmt, ...);
int								ft_vdprintf(int fd, const char *fmt,
									va_list ap);
int								ft_dprintf(int fd, const char *fmt, ...);
void							*ft_print_memory(void *addr, size_t size);

/* data constructors */
t_data							as_data(t_list_node *list_node);
t_data							as_int(int as_int);
t_data							as_size_t(size_t as_size_t);
t_data							as_char(char as_char);
t_data							as_pid_t(pid_t as_pid_t);
t_data							as_getopt_arg(int as_getopt_arg);
t_data							as_var(t_var *as_var);
t_data							as_str(char *as_str);
t_data							as_ptr(void *as_ptr);
t_data							as_literator(t_literator *as_literator);
t_data							as_token(t_token *as_token);
t_data							as_tree(t_tree *as_tree);
t_data							as_token_type(t_token_type as_token_type);
t_data							as_tree_type(t_tree_type as_tree_type);
t_data							as_gc_ptr(t_var *as_gc_ptr);
t_data							as_list(t_list *as_list);
t_data							as_str_pair(t_str_pair *as_str_pair);
t_data							as_kv_pair(t_kv_pair *as_kv_pair);

/* list */
t_list_node						*lbackward(t_list list[static 1]);
void							lprint_rev(t_list *list,
									void (print)(t_data data, int n));
bool							ldestroy(t_list list[static 1]);
void							lrotate(t_list *list, int n);
t_list							*liter(t_list list[static 1]);
t_list							*lcopy(t_list *list);
t_list							*liter_rev(t_list list[static 1]);
t_list_node						*lpush(t_list list[static 1], t_data data);
t_list_node						*lpush_left(t_list list[static 1],
									t_data data);
t_list_node						*lpop(t_list *list);
t_list_node						*ltop(t_list *list);
t_list_node						*lpop_left(t_list *list);
t_list_node						*ltop_left(t_list *list);
t_list							*lsort(t_list *list,
									bool (cmp)(t_data data1, t_data data2));
t_list							*lnew(void);
t_list_node						*lforward(t_list list[static 1]);
t_list							*lsplit(const char str[static 1],
									const char delim[static 2]);
t_list							*lsplit_n(const char str[static 1],
									const char delim[static 2], size_t n);
char							*ljoin(t_list list[static 1],
									const char delim[static 1]);
bool							lequal(t_list *list_a, t_list *list_b,
									bool (cmp)(t_data data1, t_data data2));
void							lextend_left(t_list *list_a, t_list *list_b);
void							lextend(t_list *list_a, t_list *list_b);
void							lswap(t_list *list);
t_list							*llast(t_list list[static 1]);
t_list_node						*lnext(t_list list[static 1]);
void							lprint(t_list *list,
									void (print)(t_data data, int n));
/* t_list							*lslice(t_list *list, int start, */
									/* int end, int step); */

/* hashtable */
void							ht_unset(t_ht *ht[MAX_HT_SIZE],
									char key[static 1]);
void							ht_set(t_ht *ht[MAX_HT_SIZE],
									char key[static 1], t_data data);
t_data							ht_get(t_ht *ht[MAX_HT_SIZE],
									char key[static 1]);
void							ht_print(t_ht ht[MAX_HT_SIZE],
									void (print)(char *k, t_data v));
void							ht_destroy(t_ht *ht[MAX_HT_SIZE],
									bool (*free_data)(t_data data));
t_list							*ht_to_list(t_ht *ht[MAX_HT_SIZE]);
uint64_t						ht_hash(char *key);

/* misc */
/* TODO: this function contains forbidden functions (backtrace) */
void							print_callstack(void);
void							cmt(const char *cmd_str);
size_t							ft_arrlen(char **strv);
t_list							*ft_getopt(char *const argv[],
									char valid_opts[static 1],
									char *erropt,
									int optind[static 1]);
t_list							*ft_getopt_plus(char *const argv[],
									char valid_opts[static 1],
									char *erropt,
									int optind[static 1]);

#endif /* libft.h. */
