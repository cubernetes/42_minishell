/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:09:08 by tischmid          #+#    #+#             */
/*   Updated: 2024/03/30 22:46:52 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H 1

/* TODO: remove debug include */
# include "../include/minishell_debug.h"
# include <stdarg.h>
# include <stddef.h>
# include <sys/types.h>

# define HEX_DIGITS "0123456789abcdef"
# define UHEX_DIGITS "0123456789ABCDEF"
# define NULL_PTR_STR "(null)"
# define NIL_PTR_STR "(nil)"

# define EMPTY_DEQUE "Empty deque.\n"

/* get_next_line */
# ifndef OPEN_MAX
#  define OPEN_MAX 4096
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_gnl_vars
{
	char						*prv[OPEN_MAX];
	char						*buf;
	ssize_t						b;
	int							i;
	int							j;
	int							len;
}								t_gnl_vars;

typedef enum e_bool
{
	FALSE = 0,
	TRUE
}								t_bool;

typedef struct s_list
{
	void						*content;
	struct s_list				*next;
}								t_list;


/* forward declarations */
typedef struct s_deque			t_deque;
typedef struct s_deque_node		t_deque_node;
typedef struct s_ast_node		t_ast_node;
typedef struct s_token			t_token;

struct							s_deque_node
{
	t_deque_node				*prev;
	t_deque_node				*next;
	union
	{
		void					*as_ptr;
		char					*as_str;
		t_token					*as_token;
		t_ast_node				*as_ast_node;
		long					as_long;
		int						as_int;
		char					as_char;
	};
};

struct							s_deque
{
	t_deque_node				*head;
	size_t						size;
};

typedef struct s_deque_iter
{
	t_deque						*deque;
	t_deque_node				*head;
	t_bool						_first_iter;
	t_bool						_first_iter_di_get;
}								t_di;

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
void							*ft_malloc(size_t size);
t_deque							*manage_ptrs(void *ptr);
t_bool							free_all_ptrs(void);
t_bool							ft_malloc_deque_free(t_deque *deque,
									t_bool(free_data)(void *));
t_deque							*ft_malloc_deque_init(void);
void							ft_malloc_deque_push_ptr_right(
									t_deque *deque, void *data);

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
char							*ft_strdup(char const *s);
char							*ft_strndup(char const *s, size_t len);
int								ft_strncmp(char const *s1, char const *s2,
									size_t n);
int								ft_strcmp(char const *s1, char const *s2);
int								ft_streq(char const *s1, char const *s2);
size_t							ft_strlen(char const *s);
char							**ft_split(char const *s, char c);
char							*ft_strjoin(char const *s1, char const *s2);
char							*ft_substr(char const *s, unsigned int start,
									size_t len);
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

/* math */
char							*ft_itoa(int n);
int								ft_atoi(char const *nptr);
int								ft_atoi_status(char const *nptr, int *status);
double							ft_atof(const char *s);
unsigned int					ft_abs(int n);
int								ft_max(int a, int b);
int								ft_min(int a, int b);

/* data list */
t_list							*ft_lstnew(void *content);
void							ft_lstadd_front(t_list **lst, t_list *new_head);
int								ft_lstsize(t_list *lst);
t_list							*ft_lstlast(t_list *lst);
void							ft_lstadd_back(t_list **lst, t_list *new_tail);
void							ft_lstdelone(t_list *lst, void (del)(void *));
void							ft_lstclear(t_list **lst, void (del)(void *));
void							ft_lstiter(t_list *lst, void (f)(void *));
t_list							*ft_lstmap(t_list *lst, void *(f)(void *),
									void (del)(void *));

/* printing */
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

/* deque */
t_deque_node					*deque_pop_right(t_deque *deque);
t_deque_node					*deque_pop_left(t_deque *deque);
void							deque_push_ptr_left(t_deque *deque,
									void *data);
void							deque_push_node_left(t_deque *deque,
									t_deque_node *node);
void							deque_push_ptr_right(t_deque *deque,
									void *data);
void							deque_push_node_right(t_deque *deque,
									t_deque_node *node);
void							deque_swap(t_deque *deque);
void							deque_rotate(t_deque *deque, int n);
t_deque							*deque_shallow_copy(t_deque *deque);
size_t							deque_size(t_deque *deque);
void							deque_sort(t_deque *deque, int (cmp)(void *,
										void *));
void							deque_print(t_deque *deque,
									void (print)(void *data, t_bool first));
void							deque_print_debug(t_deque *deque);
t_deque							*deque_init(void);
t_deque							*deque_shallow_slice(t_deque *deque, int start,
									int end, int step);
int								deque_index(t_deque *deque, void *data,
									t_bool(cmp)(void *, void *));
void							deque_extend_right(t_deque *deque_a,
									t_deque *deque_b);
void							deque_extend_left(t_deque *deque_a,
									t_deque *deque_b);
t_bool							deque_equal(t_deque *deque_a, t_deque *deque_b,
									t_bool(cmp)(void *, void *));
void							deque_iter(t_deque *deque,
									void (f)(void *data));
int								deque_sum(t_deque *deque, int (f)(void *data));
t_deque							*string_list_to_deque(char **array_list,
									void *(new_node)(char *str));
t_di							*di_begin(t_deque *deque);
t_deque_node					*di_get(t_di *di);
t_deque_node					*di_next(t_di *di);
/* int							deque_argmax(t_deque *deque, int *max_idx); */

/* misc */
t_bool							cmp_int_asc(int a, int b);
t_bool							cmp_int_desc(int a, int b);
char							*get_next_line(int fd);
/* TODO: this function contains forbidden functions (backtrace) */
void							print_callstack(void);
void							cmt(const char *cmd_str);

#endif /* libft.h. */
