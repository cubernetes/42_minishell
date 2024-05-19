#ifndef LIST_H
# define LIST_H 1

# include <stddef.h>
# include <stdbool.h>

/* forward declarations */
typedef struct s_list_node	t_list_node;
typedef struct s_list		t_list;
typedef struct s_token		t_token;
typedef struct s_tree		t_tree;

/** Comprehensive enumeration of data types, must match union members of t_data.
 */
enum e_type
{
	TYPE_INT,
	TYPE_SIZE_T,
	TYPE_CHAR,
	TYPE_STR,
	TYPE_PTR,
	TYPE_LITERATOR,
	TYPE_TOKEN,
	TYPE_TREE,
};

/** List iterator structure for internal use
 */
typedef struct s_literator
{
	t_list_node	*current;
	size_t		current_idx;
	t_list_node	*(*method)(t_list *list);
}	t_literator;

/** Data structure to be used with generic data structure implementations.
 */
typedef struct s_data
{
	enum e_type		type;
	union
	{
		int			as_int;
		size_t		as_size_t;
		char		as_char;
		char		*as_str;
		void		*as_ptr;
		t_literator	*as_literator;
		t_token		*as_token;
		t_tree		*as_tree;
	};
}	t_data;

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
			int			as_int;
			size_t		as_size_t;
			char		as_char;
			char		*as_str;
			void		*as_ptr;
			t_literator	*as_literator;
			t_token		*as_token;
			t_tree		*as_tree;
		};
	};
	t_list_node	*next;
	t_list_node	*prev;
};

/** List structure with `first' and `last' pointers,
 *  `len', and `current' for iteration. Other fields are private.
 */
typedef struct s_list
{
	size_t			len;
	t_list_node		*first;
	t_list_node		*last;
	t_list_node		*current;
	size_t			_current_idx; /* internal */
	t_list_node		*(*_method)(t_list *list); /* internal */
	t_list			*_iterator_stack; /* internal, don't iterate this one! */
}	t_list;

t_data          as_int(int as_int);
t_data			as_size_t(size_t as_size_t);
t_data			as_char(char as_char);
t_data			as_data(t_list_node *list_node);
t_list_node		*lbackward(t_list list[static 1]);
void			lprint_rev(t_list *list, void (print)(t_data data, bool first));
void			ldestroy(t_list list[static 1]);
void			lrotate(t_list *list, int n);
t_data			as_str(char *as_str);
t_data			as_ptr(void *as_ptr);
t_data			as_literator(t_literator *as_literator);
t_data			as_tree(t_tree *as_tree);
t_data			as_token(t_token *as_token);
t_list			*liter(t_list list[static 1]);
t_list			*lcopy(t_list *list);
t_list			*liter_rev(t_list list[static 1]);
t_list			*lpush(t_list list[static 1], t_data data);
t_list			*lpop(t_list *list);
t_list			*lpop_left(t_list *list);
void			lsort(t_list *list, bool (cmp)(t_data data1, t_data data2));
t_list			*lpush_left(t_list list[static 1], t_data data);
t_list			*lnew(void);
t_list_node		*lforward(t_list list[static 1]);
t_list			*lsplit(const char str[static 1], const char delim[static 2]);
bool			lequal(t_list *list_a, t_list *list_b, bool (cmp)(t_data data1, t_data data2));
void			lextend_left(t_list *list_a, t_list *list_b);
void			lextend(t_list *list_a, t_list *list_b);
void			lswap(t_list *list);
t_list			*llast(t_list list[static 1]);
t_list_node		*lnext(t_list list[static 1]);
void			lprint(t_list *list, void (print)(t_data data, bool first));


#endif /* list.h */
