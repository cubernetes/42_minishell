/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:09:08 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/29 03:42:01 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/* TODO: remove debug include */
# include "../include/minishell_debug.h"
# include <stdarg.h>
# include <stddef.h>
# include <sys/types.h>

# define HEX_DIGITS "0123456789abcdef"
# define UHEX_DIGITS "0123456789ABCDEF"
# define NULL_PTR_STR "(null)"
# define NIL_PTR_STR "(nil)"

# ifndef OPEN_MAX
#  define OPEN_MAX 4096
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef enum e_bool
{
	FALSE = 0,
	TRUE
}	t_bool;

typedef struct s_gnl_vars
{
	char						*prv[OPEN_MAX];
	char						*buf;
	ssize_t						b;
	int							i;
	int							j;
	int							len;
}								t_gnl_vars;

typedef struct s_list
{
	void						*content;
	struct s_list				*next;
}								t_list;

typedef struct s_deque			t_deque;
typedef struct s_deque_node		t_deque_node;

/* default */
typedef int						t_deque_type;

struct							s_deque_node
{
	t_deque_node				*prev;
	t_deque_node				*next;
	t_deque_type				data;
};

struct							s_deque
{
	t_deque_node				*head;
	size_t						size;
};

/* Quote Norm:
 * 
 * Preprocessor constants (or #define) you create must
 * be used only for literal and constant values.
 * 
 * But you don't want to allocate on the heap everytime if you know your data
 * is an int or the like. That's why we create another deque type, the
 * data deque (ddeque), where you always need to allocate on the heap
 * for the data.
 */
typedef struct s_ddeque			t_ddeque;
typedef struct s_ddeque_node	t_ddeque_node;

struct							s_ddeque_node
{
	t_ddeque_node				*prev;
	t_ddeque_node				*next;
	void						*data;
};

struct							s_ddeque
{
	t_ddeque_node				*head;
	size_t						size;
};

/* memmory */
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
char							*ft_strmapi(char const *s,
									char (f)(unsigned int, char));
void							ft_striteri(char *s, void (*f)(unsigned int,
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
void							ft_lstdelone(t_list *lst, void (*del)(void *));
void							ft_lstclear(t_list **lst, void (*del)(void *));
void							ft_lstiter(t_list *lst, void (*f)(void *));
t_list							*ft_lstmap(t_list *lst, void *(*f)(void *),
									void (*del)(void *));

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

/* regular deque (type must be define above) */
t_deque_node					*deque_pop_bottom(t_deque *deque);
t_deque_node					*deque_pop_top(t_deque *deque);
void							deque_push_value_top(t_deque *deque,
									t_deque_type data);
void							deque_push_node_top(t_deque *deque,
									t_deque_node *node);
void							deque_push_value_bottom(t_deque *deque,
									t_deque_type data);
void							deque_push_node_bottom(t_deque *deque,
									t_deque_node *node);
void							deque_swap(t_deque *deque);
void							deque_rotate(t_deque *deque, int n);
t_deque							*deque_copy(t_deque *deque);
size_t							deque_size(t_deque *deque);
void							deque_sort(t_deque *deque,
									t_bool (cmp)(t_deque_type, t_deque_type));
t_deque							*array_list_to_deque(char **array_list,
									int *status);
void							deque_print(t_deque *deque);
t_bool							deque_free(t_deque *deque);
t_deque							*deque_init(void);
t_deque							*deque_slice(t_deque *deque, int start, int end,
									int step);
int								deque_index(t_deque *deque, t_deque_type data);
void							deque_extend_free(t_deque *deque_a,
									t_deque *deque_b);
int								deque_equal(t_deque *deque_a, t_deque *deque_b);
int								deque_argmax(t_deque *deque, int *max_idx);

/* data deque */
t_ddeque_node					*ddeque_pop_bottom(t_ddeque *ddeque);
t_ddeque_node					*ddeque_pop_top(t_ddeque *ddeque);
void							ddeque_push_value_top(t_ddeque *ddeque,
									void *data);
void							ddeque_push_node_top(t_ddeque *ddeque,
									t_ddeque_node *node);
void							ddeque_push_value_bottom(t_ddeque *ddeque,
									void *data);
void							ddeque_push_node_bottom(t_ddeque *ddeque,
									t_ddeque_node *node);
void							ddeque_swap(t_ddeque *ddeque);
void							ddeque_rotate(t_ddeque *ddeque, int n);
t_ddeque						*ddeque_shallow_copy(t_ddeque *ddeque);
size_t							ddeque_size(t_ddeque *ddeque);
void							ddeque_sort(t_ddeque *ddeque, int (cmp)(void *,
										void *));
void							ddeque_print(t_ddeque *ddeque,
									void (print)(void *data, t_bool first));
t_bool							ddeque_free(t_ddeque *ddeque,
									t_bool (free_data)(void *));
t_ddeque						*ddeque_init(void);
t_ddeque						*ddeque_shallow_slice(t_ddeque *ddeque,
									int start, int end, int step);
int								ddeque_index(t_ddeque *ddeque, void *data,
									t_bool (cmp)(void *, void *));
void							ddeque_extend_free(t_ddeque *ddeque_a,
									t_ddeque *ddeque_b,
									t_bool (free_data)(void *));
t_bool							ddeque_equal(t_ddeque *ddeque_a,
									t_ddeque *ddeque_b,
									t_bool (cmp)(void *, void *));

/* misc */
t_bool							cmp_int_asc(int a, int b);
t_bool							cmp_int_desc(int a, int b);
char							*get_next_line(int fd);

#endif
