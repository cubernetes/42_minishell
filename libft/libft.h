/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:09:08 by tischmid          #+#    #+#             */
/*   Updated: 2024/05/11 16:35:19 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H 1

# include <stdarg.h>
# include <stddef.h>
# include <sys/types.h>
# include <stdbool.h>
# include <stdint.h>

# define HEX_DIGITS "0123456789abcdef"
# define UHEX_DIGITS "0123456789ABCDEF"
# define NULL_PTR_STR "(null)"
# define NIL_PTR_STR "(nil)"

# define EMPTY_DEQUE "Empty deque.\n"

#define UNIQUE1 "<START_UNIQUE>"
#define UNIQUE2 "<END_UNIQUE>"

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

/* hashtable */
typedef struct s_deque_iter
{
	t_deque						*deque;
	t_deque_node				*head;
	bool						_first_iter;
	bool						_first_iter_di_get;
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
t_deque							*gc_add(void *ptr);
char							*gc_add_str(void *ptr);
t_deque							*gc_set_null(void **ptr);
bool							gc_free(void);
bool							ft_malloc_deque_free(t_deque *deque,
									bool(free_data)(void *));
t_deque							*ft_malloc_deque_init(void);
void							ft_malloc_deque_push_ptr_right(
									t_deque *deque, void *data);
void							*gc_malloc(size_t size);
void							*(*set_allocator(void *(*_allocator)
										(size_t size)))(size_t size);
void							*(*get_allocator(void))(size_t size);

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
bool							cmp_int_asc(int a, int b);
bool							cmp_int_desc(int a, int b);
char							*ft_itoa(int n);
int								ft_atoi(char const *nptr);
int								ft_atoi_status(char const *nptr, int *status);
double							ft_atof(const char *s);
unsigned int					ft_abs(int n);
int								ft_max(int a, int b);
int								ft_min(int a, int b);

/* io */
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
									void (print)(void *data, bool first));
void							deque_print_debug(t_deque *deque);
t_deque							*deque_init(void);
t_deque							*deque_shallow_slice(t_deque *deque, int start,
									int end, int step);
int								deque_index(t_deque *deque, void *data,
									bool(cmp)(void *, void *));
void							deque_extend_right(t_deque *deque_a,
									t_deque *deque_b);
void							deque_extend_left(t_deque *deque_a,
									t_deque *deque_b);
bool							deque_equal(t_deque *deque_a, t_deque *deque_b,
									bool(cmp)(void *, void *));
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
/* TODO: this function contains forbidden functions (backtrace) */
void							print_callstack(void);
void							cmt(const char *cmd_str);

#endif /* libft.h. */
